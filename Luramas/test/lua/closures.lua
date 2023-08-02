
-- Global closure
function global_function(a1, a2, a3, ...)

    local function scoped_function(a1, ...)

    end

    -- Callback
    a1(...);

end

global_function((function(a1) print(a1) end) --[[ lambda ]], 1, 2, 3, 4, 5);


