#include "gui/constraints.hpp"

bool Constraints::valid() const {
    if (x.getType() == ConstraintType::ASPECT || y.getType() == ConstraintType::ASPECT) {
        return false;
    }

    if (x.getType() == ConstraintType::FLEX || y.getType() == ConstraintType::FLEX) {
    }

    if (width.getType() == ConstraintType::ASPECT && height.getType() == ConstraintType::ASPECT) {
        return false;
    }

    if (width.getType() == ConstraintType::CENTER || height.getType() == ConstraintType::CENTER) {
        return false;
    }

    return true;
}

template<> const AbsoluteConstraint& Constraint::getValue() const {
    return absolute;
}

template<> const RelativeConstraint& Constraint::getValue() const {
    return relative;
}

template<> const AspectConstraint& Constraint::getValue() const {
    return asp;
}

template<> const FlexConstraint& Constraint::getValue() const {
    return flex;
}

template<> AbsoluteConstraint& Constraint::getValue() {
    return absolute;
}

template<> RelativeConstraint& Constraint::getValue() {
    return relative;
}

template<> AspectConstraint& Constraint::getValue() {
    return asp;
}

template<> FlexConstraint& Constraint::getValue() {
    return flex;
}
