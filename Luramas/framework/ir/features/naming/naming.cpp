#include "naming.hpp"

namespace data {

      /* 
	
		Legend:
			"tb-idx" -> Table index.
			"fn-ret" -> Call return.
			"fn-par" -> Call parameter.
			"it-max" -> Used as max iteration variable.
			"it-var" -> Used as iteration variable.
			"dl[??]" -> Data type list.


			"B" - boolean
			"T" - tables
			"S" - string
			"F" - function


		    * l = Lvalue
			* r = Rvalue
			

			"-o" = only
			"-oo" = only once
			"-c" = canidate for it if hit all
			"!" = not
			"-nu" = never used
			"/" = or
			"{??}" = at least for list
			"&" = and


	*/
      static constexpr const char *const names[] = {

          "data_space",    /* -c (l)it-max & (r)it-var  */
          "max_size",      /* -c (l)it-max */
          "unused_retn",   /* -o -nu (l)fn-ret */
          "volatile_data", /* dl[B/T/S/F]{3} */
          "table",         /* */

      };

} // namespace data