#pragma once
#include <cstdint>
namespace luramas {

      namespace ast {

            /* Types of closures. */
            enum class closure_kinds : std::uint8_t {
                  nothing,
                  main,
                  scoped,
                  global,
                  anonymous
            };

            /* 
      
            * Expr/Stat(important ones, Descriptor exists) type. {desc, expr, stat or descriptor?, intended usage/set for mostly}
            * Both AST exprs, stat, discriptors are used in the same enum.
            * Both get treated differently but relatively the same so the same enum makes it more efficient for searching etc.
            * Stat and Exprs, etc arent identified by enum name but what it comment it. (Does not cause a huge problem because of how it is setup but it does make it look cleaner),
        
      */
            enum class element_kinds : std::uint8_t {

                  lex, /* Specific type refer to lexer. (DESCRIPTOR) [PLACEHOLDER] */

                  /* Set following return. */
                  exit_post, /* Exits decompilation after instruction. (DESCRIPTOR) [LIFTER] */
                  exit_pre,  /* Exits decompilation after instruction. (DESCRIPTOR) [LIFTER] */
                  exit_dead, /* Instruction exits because of a return. (DESCRIPTOR) [AST] */

                  arith,  /* r1 += r1 + r1 (EXPR) [AST] */
                  arithK, /* r1 += r1 + 1 (EXPR) [AST] */

                  /* Generic locvar */
                  locvar,         /* Dest is locvar. (STAT) [ALL] */
                  locvar_upvalue, /* Dest locvar turns into a upvalue. (STAT) [ALL] */
                  locvar_multret, /* Dest is locvar(Multret). (STAT) [ALL] */

                  /* Doesn't garunteed arg/var or anything. */
                  statement_begin,  /* Begin statement line. (DESCRIPTOR) [AST] */
                  statement_member, /* Member of statement line. (DESCRIPTOR) [AST] */
                  statement_end,    /* End statement line. (DESCRIPTOR) [AST] */
                  statement_single, /* Member of statement single. (DESCRIPTOR) [AST] */

                  table, /* Table instruction. [AST] */

                  /* Multret concat **Applicable too move, set(table/global/upv)** */
                  call_multret_start,  /* Call multret concat start (DESCRIPTOR) [LIFTER] */
                  call_multret_member, /* Call multret concat member (DESCRIPTOR) [LIFTER] */
                  call_multret_end,    /* Call multret concat end (DESCRIPTOR) [LIFTER] */

                  /* Constants */
                  for_iv_start, /* for i,v in pairs ({ 1 }) do (STAT) [ALL] */
                  for_iv_end,   /* for(i,v) end(jumpback) (STAT) [AST] */
                  for_start,    /* for generic start, serves as jump too OP_FORLOOPG (follows format). (STAT) [ALL] */
                  for_end,      /* for generic end(jumpback) (STAT) [AST] */
                  for_n_start,  /* for numeric start, serves as jump too OP_FORLOOPN (follows format). (numeric) (STAT) [ALL] */
                  for_n_end,    /* for numeric end(jumpback) (STAT) [AST] */
                  for_prep,     /* for preperation instruction (STAT) [AST] */

                  /* Goes by order */
                  source_outside_scope, /* Source register declared outside of scope that isn't an argument. (DESCRIPTOR) [ALL] */
                  source_inside_scope,  /* Source register declared inside of scope. (Can also mean arg) (DESCRIPTOR) [ALL] */

                  /* Constants */
                  repeat_,   /* repeat (STAT) [ALL] */
                  while_,    /* while () follows condition(not jumpback). (STAT) [ALL] */
                  while_end, /* while () end(jumpback) (STAT) [AST] */
                  until_,    /* until () follows condition(typically jumpback). (STAT) [ALL] */
                  break_,    /* break (STAT) [ALL] */
                  scope_end, /* scope end (generic) (STAT) [ALL] */

                  call_routine_start, /* Call routine start. (DESCRIPTOR) [AST] */
                  call_routine_end,   /* Call routine end. (DESCRIPTOR) [AST] */

                  concat_routine_start, /* Concat routine start. (DESCRIPTOR) [AST] */
                  concat_routine_end,   /* Concat routine end. (DESCRIPTOR) [AST] */

                  conditional_expression_predicted, /* Indication that this may be the end of a conditional_expression. (Does not garunteed one) (DESCRIPTOR) [AST] */
                  conditional_expression_start,     /* Conditional expression routine start. (DESCRIPTOR) [AST] */
                  conditional_expression_end,       /* Conditional expression routine end. (Compare flag gets sets in dest(Hard coded else for OP_LOADBOOL/OP_GETTABUPVALUE(no source register))) (DESCRIPTOR) [AST] */
                  conditional_expression_end_emit,  /* Same as conditonal_expression_end but it emits data too the end of compare flag (EXPR) [LIFTER]*/

                  conditonal_jumps_out, /* Condition jumps out of current scope (Ignores break). (DESCRIPTOR) [AST] */
                  conditional_break,    /* Condition breaks out of loop but doesn't garunteed a break. (DESCRIPTOR) [AST] */

                  /* Some constants */
                  if_,                     /* if () (STAT) [ALL] */
                  elseif_,                 /* elseif () (STAT) [ALL] */
                  else_,                   /* else (STAT) [ALL] */
                  condition_nonmutable,    /* Condition that cannot be converted into while/if/elseif etc. (DESCRIPTOR) [AST] */
                  condition_concat_start,  /* Concat a condition(universal) (start). (DESCRIPTOR) [AST] */
                  condition_concat_member, /* Concat a condition(universal) (member). (DESCRIPTOR) [AST] */
                  condition_concat_end,    /* Concat a condition(universal) (end will get written too compare flag). (DESCRIPTOR) [AST] */
                  condition_true,          /* Sets compare flag too true garunteing that while true expressions get set as true (expr type). (EXPR) [ALL] */
                  condition_flag,          /* Writes result to flag. (DESCRIPTOR) [LIFTER] */
                  condition_break,         /* Conditon leads too break. (Safer than break, end exprs. Garunteeds "break; \n end" emit) (STAT) [LIFTER] */
                  condition_emit_next,     /* Emits compare data too dest register in next instruction. (DESCRIPTOR) [LIFTER] */
                  /* Different from concat condition doesn't garunteed actual concatation just a hint. */
                  condition_logical_start, /* Start of a logical operation. (DESCRIPTOR) [AST] */
                  condition_logical,       /* Apart of a logical operation. (DESCRIPTOR) [AST] */
                  condition_logical_end,   /* End of a logical operation. (DESCRIPTOR) [AST] */
                  condition_append_source, /* Appends source too flag compare flag. (DESCRIPTOR) [LIFTER] */

                  /* Will get emmited to condition flag post compare. */
                  condition_and,              /* if/elseif/nested(and) appends to if_statements (Can be applied to until or while) (EXPR) [ALL] */
                  condition_or,               /* if/elseif/nested(or)  appends to if_statements (Can be applied to until or while) (EXPR) [ALL] */
                  condition_end_scope,        /* Conditon flag end scope:  %s ) [ALL] */
                  condition_start_scope,      /*  Conditon flag start scope(PRE): ( %s   [ALL] */
                  condition_start_scope_post, /*  Conditon flag start scope(POST): ( %s   [ALL] */

                  /* These only apply to routines outside of other routines like, call, concat, table, etc. (Will account for call parameters) [AST] */
                  condition_routine, /* Means instruction is apart of a conditional routine (will only account for valid data not known args or vars, some will get in like if instruction before branch is locvar) **Does not mean it can't be an arguement!!** [AST] */
                  condition_routine_start,
                  condition_routine_end,

                  /* Jump */
                  jump_elseif, /* Jump leads too elseif (DESCRIPTOR) [AST] */
                  jump_else,   /* Jump leads too else (DESCRIPTOR) [AST] */

                  table_start,   /* Table { (EXPR) [ALL] */
                  table_element, /* Element in table. (Not usable for setlist cause of concatation) (EXPR) [ALL] */
                  table_end,     /* Table } (Will get ignored and use SETLIST instruction integral operand amt if it hits SETLIST.) (EXPR) [ALL] */
                  table_index,   /* Extra expr used for certain things (Will be appended when everything is done). (EXPR) [ALL] */

                  closure_scoped,    /* Scoped function ?? () **Can be mutated by lv set if it is a lv will be scoped else anonymous** (STAT) [ALL] */
                  closure_global,    /* Global function ?? () **Can be mutated by lv set if it is a lv instead of a global will turn into scoped** (STAT) [ALL] */
                  closure_anonymous, /* Anonymous (STAT) [ALL] */

                  bad_instruction,  /* Instruction will never get executed no matter watch branch is taken or not. (DESCRIPTOR) [AST] */
                  dead_instruction, /* Instruction gets ignored. *Will run exprs but not instruction in lifter. (DESCRIPTOR) [LIFTER] */
                  conditional       /* Condition flag will get written too dest. (Used for branching opcodes including loadb +jmp **Will clear compare flag if conditional is not loadb) (DESCRIPTOR) [ALL] */

            };

            /* Binary operation kinds. */
            enum class bin_kinds : std::uint8_t {
                  nothing,

                  /* Arith */
                  add_,  /* (+) */
                  sub_,  /* (-) */
                  mul_,  /* (*) */
                  div_,  /* (/) */
                  idiv_, /* (//) */
                  mod_,  /* (*) */
                  pow_,  /* (^) */
                  and_,  /* (&) */
                  xor_,  /* (~) */
                  shl_,  /* (<<) */
                  shr_,  /* (>>) */
                  or_,   /* (|) */

                  /* Unary */
                  len_,    /* Lenght of array/vector/table. */
                  minus_,  /* Unary minus */
                  not_,    /* Unary not */
                  bitnot_, /* Unary bitnot */
                  plus_,   /* Unary plus */

                  /* Compares, for both set cmp flags and jmps with compare. */
                  eq_,  /* (==) */
                  ne_,  /* (!=) */
                  lt_,  /* (<) */
                  lte_, /* (<=) */
                  gt_,  /* (>) */
                  gte_  /* (>=) */
            };

      } // namespace ast

} // namespace luramas