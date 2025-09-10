#pragma once

#include "Order.hpp"
#include <variant>

namespace tme {
        // Add new types of actions as needed. 
        struct NewOrder {Order order; };

        // Add new actions here as required.
        using Command = variant<NewOrder>;
}