#pragma once
#include "../../ir.hpp"
//#include <llvm/IR/Function.h>
//#include <llvm/IR/IRBuilder.h>
//#include <llvm/IR/LLVMContext.h>
//#include <llvm/IR/LegacyPassManager.h>
//#include <llvm/IR/Module.h>
//#include <llvm/IR/PassManager.h>
//#include <llvm/Transforms/Scalar.h>

namespace luramas {

      namespace ir {

            namespace optimize {

                  struct optimization_options {

                        struct loops {

                              struct unroll {

                                    bool enable = true;

                                    std::int32_t optlevel = 2;
                                    bool onlywhenforced = false;
                                    bool forgetallscev = false;

                              } unroll;

                        } loops;

                        bool disable_unroll_loops = false;
                        bool disable_deadcode_elimination = false;
                        bool disable_deadstore_elimination = false;
                        bool disable_function_inlining = false;
                        bool disable_arith_reassociation = false;
                        bool disable_simplify_cfg = false;
                  };

                  /* Optimises IR */
                  //void set_passes(std::shared_ptr<luramas::ir::optimize::optimization_options> &options, llvm::Module &mod) {
                  //
                  //      //auto func_pass = new llvm::FunctionPassManager;
                  //      //auto pass = llvm::legacy::PassManager {};
                  //
                  //      if (!options->disable_deadcode_elimination) {
                  //            //  func_pass->addPass(llvm::createDeadCodeEliminationPass());
                  //      }
                  //      //pass.add(llvm::createSimpleLoopUnrollPass());
                  //
                  //      //if (!options->disable_unroll_loops) {
                  //      //    retn.emplace_back(llvm::createSimpleLoopUnrollPass(options->loops.unroll.optlevel, options->loops.unroll.onlywhenforced, options->loops.unroll.forgetallscev));
                  //      //}
                  //      //
                  //      //if (!options->disable_deadstore_elimination) {
                  //      //    retn.emplace_back(llvm::createDeadStoreEliminationPass());
                  //      //}
                  //      //
                  //      //if (!options->disable_arith_reassociation) {
                  //      //    retn.emplace_back(llvm::createReassociatePass());
                  //      //}
                  //      //
                  //      //if (!options->disable_simplify_cfg) {
                  //      //    retn.emplace_back(llvm::createCFGSimplificationPass());
                  //      //}
                  //      //
                  //      //return retn;
                  //}

            } // namespace optimize

      } // namespace ir

} // namespace luramas