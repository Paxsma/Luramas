#include "example.hpp"

#if luramas_target == luramas_target_luau_v3

/* Compiles lua script and returns proto. */
Proto *compile_script(const std::string &code, bool &error, lua_State *&buffer) {

      /* Compilation data */
      std::size_t size = 0u;    /* Size buffer of compiled code. */
      buffer = luaL_newstate(); /* New lua state */

      /* Load see if theres something wrong. */
      if (luau_load(buffer, "TEST", luau_compile(code.c_str(), std::strlen(code.c_str()), NULL, &size), size, 0)) {
            error = true; /* Return error */
            return NULL;
      }

      return gco2cl((buffer->top - 1)->value.gc)->l.p; /* Code proto. */
}

std::string luramas::decompile(const std::string &code, const std::shared_ptr<ir::lifter::lifter_config> &config, std::shared_ptr<luramas::ir::data::format::format> &format, std::shared_ptr<luramas::ir::data::optimizations::optimize> &optimizations) {

      static const auto syntax_target = luramas::emitter_ir::syntax::emitter_syntax::luau; /* Syntax target */
      bool error = false;                                                                  /* Error buffer */

      /* Check for unique prefixes. */
      if (!config->unique()) {
            throw std::runtime_error("One of your prefixes in config are ambigous please make them all unique.");
      }

      lua_State *original_ls = nullptr;
      lua_State *decomp_ls = nullptr;

      auto proto = compile_script(code, error, original_ls); /* Compile code. */

      /* Error in code compilation. */
      if (error) {
            lua_close(original_ls);
            throw std::runtime_error("Error in compiling code.");
      }

      auto il = luramas::il::lifter::lift(proto);          /* Generate IL */
      luramas::il::transformers::eliminate::junk_code(il); /* Eliminate junk code */

      auto ast = luramas::ast::gen_ast(syntax_target, il, config, format); /* Generate AST */
      ast->run<true>(); /* Run passthroughs */
      auto ir = luramas::ir::lifter::lift(ast, config, format, optimizations);                  /* Lift AST too IR. */
      auto ir_map = ir.get_map();                                                               /* Get IR map. */
      auto decom = luramas::ir::generate(ast->syntax, ir.get(), ir_map, format, optimizations); /* Generate code from IR. */

      if (config->post_loss) {

            auto decomp_proto = compile_script(decom, error, decomp_ls); /* Compiled decompiled code. */
            auto decomp_il = luramas::il::lifter::lift(decomp_proto);    /* Generate decompiled IL */

            /* Error in code compilation. */
            if (error) {

                  /* Emit error too code header. */
                  luramas::emitter_ir::common::comment::expandable_comment<false>(syntax_target, decom, "Unable to compile decompilation!", format, 0u, format->linebreak.linebreak_expandable_comment_pre, format->linebreak.linebreak_expandable_comment_post, !format->linebreak.disable);

            } else {

                  const auto decomp_ast = luramas::ast::gen_ast(syntax_target, decomp_il, config, format); /* Generate AST. */
                  const auto loss = luramas::loss::loss(ast, decomp_ast);                                  /* Calculate Loss. */

                  /* Emit loss too header. */
                  luramas::emitter_ir::common::comment::expandable_comment<false>(syntax_target, decom, ((loss < 0) ? std::string("Loss: " + std::to_string(loss) + "%") : (loss != 0) ? std::string("Gain: " + std::to_string(loss) + "%")
                                                                                                                                                                                       : std::string("Perfect decompilation!")),
                      format,
                      0u,
                      format->linebreak.linebreak_expandable_comment_pre,
                      format->linebreak.linebreak_expandable_comment_post,
                      !format->linebreak.disable);
            }

            /* Close lua state. */
            lua_close(decomp_ls);
      }

      /* Clean up. */
      luramas::ir::ir_expr_data::cache::deconstruct::deconstruct(); /* Destroy all IR pointers. */

      /* Close lua state. */
      lua_close(original_ls);

      return decom;
}

#endif