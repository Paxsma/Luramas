#pragma once
#include "features/formatting.hpp"
#include "features/optimizations.hpp"
#include "features/refrence_string.hpp"
#include <iterator>
#include <string>
#include <vector>

namespace luramas {

      namespace ir {

            namespace data {

                  enum class kinds : std::uint8_t {
                        expression,
                        comment,
                        expandable_comment, /* Garunteeds an expandable comment. Does not concat no matter what. */
                        statement
                  };

                  namespace expr {

                        enum class element_kinds : std::uint8_t {
                              arith,
                              call,
                              data
                        };

                        enum class refrence_kinds : std::uint8_t {
                              arith,    /* Arith */
                              integer,  /* Integer */
                              boolean,  /* Boolean */
                              none,     /* None */
                              kval,     /* Kvalue */
                              selfcall, /* Selfcall */
                              var,      /* Variable/Upvalue */
                              arg,      /* Arguement */
                              global,   /* Global */
                              upvalue,  /* Upvalue */
                              vararg,   /* Vararg */
                              string    /* String */
                        };

                  } // namespace expr

                  namespace data {

                        /* Data kind. */
                        enum class data_kinds : std::uint8_t {

                              nothing,

                              integer,      /* Integer */
                              boolean,      /* Boolean */
                              none,         /* None object */
                              kval,         /* Kvalue */
                              var,          /* Variable/Upvalue */
                              arg,          /* Arguement */
                              global,       /* Global */
                              vararg,       /* "..." */
                              selfcall,     /* Selfcall */
                              idx,          /* Index "[]" or secondary index */
                              idx_generic,  /* Specific for index. "." or primary index */
                              upvalue,      /* Upvalue */
                              call,         /* Call */
                              concat,       /* Concat */
                              concat_exprs, /* Extra exprs that pair with concat. e.g. arith, compare, etc */
                              string        /* String */
                        };

                        /* For lvalues. e.g. a1 = a2 + a3; -- "a2 +" a2(lvalue expr register with concat of arith = "+") *Expr types kinds */
                        enum class concat_data_kinds : std::uint8_t {
                              nothing,
                              arith,   /* +,-,^,*,%,/,&,| */
                              compare, /* &&(and),||(or),<,<=,==,~=,>,>= */
                              table    /* { ?? } */
                        };

                        /* Type kinds */
                        enum class type_kinds : std::uint8_t {
                              nothing,
                              call,
                              logical
                        };

                  } // namespace data

                  namespace constant {

                        /* Logical operation kinds */
                        enum class logical_operations_kinds : std::uint8_t {
                              nothing, /* Nothing */
                              and_,    /* && */
                              or_,     /* || */
                              lt,      /* < */
                              lte,     /* <= */
                              eq,      /* == */
                              ne,      /* ~= */
                              gt,      /* > */
                              gte,     /* >= */
                              nt,      /* ! */
                        };

                        enum class constant_kinds : std::uint8_t {

                              nothing, /* not constant */

                              do_,     /* do */
                              while_,  /* while (Will only recognize 1 compare **Concat for more**) */
                              repeat_, /* repeat */
                              until,   /* repeat until(??) (Will only recognize 1 compare **Concat for more**) */

                              for_,        /* for loop (GENERIC) */
                              for_numeric, /* for loop (numeric) */

                              if_,     /* if statement (Will only recognize 1 compare **Concat for more**) */
                              elseif_, /* else if (Will only recognize 1 compare **Concat for more**) */
                              else_,   /* else */
                              end_,    /* scope end */

                              break_,    /* break */
                              continue_, /* continue */
                              function_, /* type deduced by constant(global function/scoped function/anonymous function) */
                              return_,   /* return */

                              compares /* Not really a constant but it is facilitated by constant being in compares. */

                        };

                        /* Function kinds */
                        enum class function_kinds : std::uint8_t {
                              nothing,
                              anonymous,
                              scoped,
                              global
                        };

                  } // namespace constant

                  namespace data_refrence {

                        struct table_refrence_member {

                              bool table_start = false; /* { */
                              bool table_end = false;   /*  } */

                              refrence_string index = "";
                              ir::data::refrence_string value = "";
                        };

                  } // namespace data_refrence

            } // namespace data

      } // namespace ir

} // namespace luramas