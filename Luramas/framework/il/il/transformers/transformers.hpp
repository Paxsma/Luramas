/*
* framework/il/il/transformers/transformers.hpp
* 
* IL Transformers
* 10/7/2023
* 
* Provides the tools that allows users to transform IL.
*/
#pragma once

#ifndef luramas_il_transformers
#define luramas_il_transformers

#include "../il.hpp"

namespace luramas {

      namespace il {

            namespace transformers {

                  namespace eliminate {

                        /* Eliminate junk code i.e. nops without label. */
                        void junk_code(const std::shared_ptr<luramas::il::ilang> &il);

                  } // namespace eliminate

            } // namespace transformers

      } // namespace il

} // namespace luramas

#endif