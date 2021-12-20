#include "gui/constraints.hpp"

bool Constraints::valid() const {
    if (x.type == ConstraintType::ASPECT || y.type == ConstraintType::ASPECT) {
        return false;
    }

    if (width.type == ConstraintType::ASPECT && height.type == ConstraintType::ASPECT) {
        return false;
    }

    if (width.type == ConstraintType::CENTER || height.type == ConstraintType::CENTER) {
        return false;
    }
}