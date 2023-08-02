#include "smart_formatting.hpp"

/* Smart format */
void luramas::smart_formatting::smart_format(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::lines &buffer, std::shared_ptr<luramas::ir::data::format::format> &format) {

      /* Disabled */
      if (format->smart_format.disabled) {
            return;
      }

#pragma region on

      struct statement_indent {

            bool on = false;
            std::size_t start = 0u;
            std::size_t start_idx = 0u;
            std::size_t count = 0u;
            std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> data = nullptr;

            void reset() {
                  this->on = false;
                  this->start = 0u;
                  this->start_idx = 0u;
                  this->count = 0u;
                  this->data = nullptr;
                  return;
            }
      };

      statement_indent settable;
      statement_indent selfcall;

#pragma endregion

      const auto lines = buffer.get();
      for (auto i = 0u; i < lines.size(); ++i) {

            const auto line = lines[i];

            /* Not statement */
            if (line->line_type != luramas::ir::data::kinds::statement || line->statement.lvalue.empty()) {
                  continue;
            }

            const auto lvalue = line->statement.lvalue.front();

            /* settable */
            if (!format->smart_format.settable.disabled) {

                  if ((lvalue->exprs.datatype_kind == luramas::ir::data::data::data_kinds::idx || lvalue->exprs.datatype_kind == luramas::ir::data::data::data_kinds::idx_generic) && (settable.data == nullptr || settable.data->cmp(syn, lvalue->exprs.data.idx.table, format) /* Settable nullptr or same. */)) {

                        /* First */
                        if (settable.data == nullptr) {
                              settable.start_idx = i;
                              settable.data = lvalue->exprs.data.idx.table;
                        }

                        /* Mutate for treshold. */
                        if (!format->smart_format.settable.min_threshold || (++settable.start) >= format->smart_format.settable.min_threshold) {

                              if (!format->smart_format.settable.max_threshold || settable.start <= format->smart_format.settable.max_threshold) {

                                    /* New */
                                    if (!settable.on && !buffer.key_exist(settable.start_idx)) {
                                          std::string newlines = "";
                                          luramas::emitter_ir::common::line::emit_linebreak(syn, newlines, format->smart_format.settable.group_newlines, format);
                                          buffer.insert_map(settable.start_idx, newlines);
                                    }

                                    settable.on = true;
                                    ++settable.count;

                              } else {
                                    goto settable_end; /* Maxed out max jump too end. */
                              }
                        }

                  } else {

                  settable_end:

                        /* Not index and on settable so end. */
                        if (settable.on) {
                              std::string newlines = "";
                              luramas::emitter_ir::common::line::emit_linebreak(syn, newlines, format->smart_format.settable.group_newlines, format);
                              buffer.insert_map(i - 1u, newlines);
                        }

                        settable.reset();

                        /* Set for possible target. */
                        if (lvalue->exprs.datatype_kind == luramas::ir::data::data::data_kinds::idx || lvalue->exprs.datatype_kind == luramas::ir::data::data::data_kinds::idx_generic) {
                              settable.start_idx = i;
                              settable.data = lvalue->exprs.data.idx.table;
                              ++settable.start;
                        }
                  }
            }
      }

      return;
}