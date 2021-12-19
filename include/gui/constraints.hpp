#pragma once

enum class ConstraintType
{
    ABSOLUTE,
    RELATIVE,
    ASPECT
};

struct Constraint {
    float value;

    ConstraintType type;

    inline Constraint(float value, ConstraintType type)
        : value(value), type(type) {
    }
};

struct AbsoluteConstraint : public Constraint {
    inline AbsoluteConstraint(float value)
        : Constraint(value, ConstraintType::ABSOLUTE) {
    }
};

struct RelativeConstraint : public Constraint {
    inline RelativeConstraint(float value)
        : Constraint(value, ConstraintType::RELATIVE) {
    }
};

struct AspectConstraint : public Constraint {
    inline AspectConstraint(float value)
        : Constraint(value, ConstraintType::ASPECT) {
    }
};

struct Constraints {
    Constraint x;
    Constraint y;
    Constraint width;
    Constraint height;

    inline Constraints()
        : x{0, ConstraintType::ABSOLUTE},
          y{0, ConstraintType::ABSOLUTE},
          width{1, ConstraintType::RELATIVE},
          height{1, ConstraintType::RELATIVE} {
    }

    bool valid() const;
};
