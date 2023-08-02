#include "example.hpp"

#if luramas_target == luramas_target_lua_53_6

/* Compiles lua script and returns proto. */
Proto *compile_script(const std::string &code, bool &error, lua_State *&buffer) {

      buffer = luaL_newstate(); /* New lua state */

      if (luaL_loadstring(buffer, code.c_str()) != LUA_OK) {
            error = true; /* Return error */
            return NULL;
      }

      return gco2cl((buffer->top - 1)->value_.gc)->l.p; /* Code proto. */
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

      const auto proto = compile_script(code, error, original_ls); /* Compile code. */

      for (auto pc = 0u; pc < unsigned(proto->sizecode);) {
            auto dism = std::make_shared<lua_53_6_disassembler::disassembly>();
            lua_53_6_disassembler::disassemble(pc, proto, dism);
            pc += dism->len;
      }

      std::cin.get();
      /* Error in code compilation. */
      if (error) {
            lua_close(original_ls); /* Close lua state. */
            throw std::runtime_error("Error in compiling code.");
      }

      auto il = luramas::il::lifter::lift_proto(proto);                                         /* Generate IL */
      auto ast = luramas::ast::gen_ast(syntax_target, il, config, format);                      /* Generate AST */
      auto ir = luramas::ir::lifter::lift(ast, config, format, optimizations);                  /* Lift AST too IR. */
      auto ir_map = ir.get_map();                                                               /* Get IR map. */
      auto decom = luramas::ir::generate(ast->syntax, ir.get(), ir_map, format, optimizations); /* Generate code from IR. */

      if (config->post_loss) {

            auto decomp_proto = compile_script(decom, error, decomp_ls);    /* Compiled decompiled code. */
            auto decomp_il = luramas::il::lifter::lift_proto(decomp_proto); /* Generate decompiled IL */

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