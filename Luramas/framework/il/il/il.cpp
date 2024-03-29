#include "il.hpp"
#include "architecture/emitter.hpp"

template <typename t>
t read(std::int8_t *&il) {
      t retn = 0;
      std::memcpy(&retn, il, sizeof(t));
      il += sizeof(t);
      return retn;
}

void luramas::il::disassemble_il_bytecode(void *il_bytecode, std::shared_ptr<luramas::il::ilang> &buffer) {

      auto il = static_cast<std::int8_t *>(il_bytecode);

      /* Read kcode amount. */
      const auto kcode_amt = read<std::uint64_t>(il);

      /* Disssassemble kvalues */
      for (auto i = 0u; i < kcode_amt; ++i) {

            const auto kval_kind = static_cast<il::arch::kval_kinds>(read<std::uint8_t>(il));

            auto kval = std::make_shared<il::ilang::kvalue>();

            switch (kval_kind) {

                  case il::arch::kval_kinds::boolean: {
                        kval->boolean.b = read<std::uint8_t>(il);
                        break;
                  }

                  case il::arch::kval_kinds::closure: {
                        kval->closure.id = read<std::uint32_t>(il);
                        break;
                  }

                  case il::arch::kval_kinds::function: {
                        const auto len = read<std::uint32_t>(il);

                        break;
                  }

                  case il::arch::kval_kinds::integer: {
                        kval->integer.i = read<double>(il);
                        kval->integer.str = std::to_string(kval->integer.i);
                        kval->integer.str.erase(kval->integer.str.find_last_not_of('0') + 1, std::string::npos);
                        kval->integer.str.erase(kval->integer.str.find_last_not_of('.') + 1, std::string::npos);
                        break;
                  }

                  default: {
                        throw std::runtime_error("Unkown kvalue type.");
                  }
            }

            buffer->kval.emplace_back(kval);
      }

      return;
}
