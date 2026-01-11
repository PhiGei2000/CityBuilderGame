#pragma once

enum class ConstraintType {
    ABSOLUTE,
    RELATIVE,
    ASPECT,
    CENTER,
    FIT_TO_CONTENT
};

struct Constraint {
    float value;

    ConstraintType type;

    inline Constraint(float value, ConstraintType type)
        : value(value), type(type) {
    }
};

struct AbsoluteConstraint : public Constraint {
    inline AbsoluteConstraint(float value = 0.0f)
        : Constraint(value, ConstraintType::ABSOLUTE) {
    }
};

struct RelativeConstraint : public Constraint {
    inline RelativeConstraint(float value = 1.0f)
        : Constraint(value, ConstraintType::RELATIVE) {
    }
};

struct AspectConstraint : public Constraint {
    inline AspectConstraint(float value)
        : Constraint(value, ConstraintType::ASPECT) {
    }
};

struct CenterConstraint : public Constraint {
    inline CenterConstraint()
        : Constraint(0, ConstraintType::CENTER) {
    }
};

struct FitToContentConstraint : public Constraint {
    inline FitToContentConstraint()
        : Constraint(0, ConstraintType::FIT_TO_CONTENT) {
    }
};

struct Constraints {
    Constraint x;
    Constraint y;
    Constraint width;
    Constraint height;

    inline Constraints()
        : x{0, ConstraintType::CENTER},
          y{0, ConstraintType::CENTER},
          width{1, ConstraintType::RELATIVE},
          height{1, ConstraintType::RELATIVE} {
    }

    bool valid() const;
};
