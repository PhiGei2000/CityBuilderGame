#pragma once
#include <tuple>

enum class ConstraintType {
    UNDEFINED,
    ABSOLUTE,
    RELATIVE,
    ASPECT,
    FLEX,
    CENTER,
    FIT_TO_CONTENT
};

struct ConstraintBase {
};

struct ValueConstraintBase : public ConstraintBase {
    float value;

    inline ValueConstraintBase(float value)
        : value(value) {
    }
};

struct AbsoluteConstraint : public ValueConstraintBase {
    inline AbsoluteConstraint(float value = 0.0f)
        : ValueConstraintBase(value) {
    }
};

struct RelativeConstraint : public ValueConstraintBase {
    inline RelativeConstraint(float value = 1.0f)
        : ValueConstraintBase(value) {
    }
};

struct AspectConstraint : public ValueConstraintBase {
    inline AspectConstraint(float value)
        : ValueConstraintBase(value) {
    }
};

struct FlexConstraint : public ValueConstraintBase {
    float absoluteValue = 0.0f;

    inline FlexConstraint(float value)
        : ValueConstraintBase(value) {
    }
};

struct CenterConstraint : public ConstraintBase {
    inline CenterConstraint() {
    }
};

struct FitToContentConstraint : public ConstraintBase {
    inline FitToContentConstraint() {
    }
};

struct Constraint {
  private:
    ConstraintType type;
    union {
        AbsoluteConstraint absolute;
        RelativeConstraint relative;
        AspectConstraint asp;
        FlexConstraint flex;
    };

  public:
    inline Constraint(const AbsoluteConstraint& absolute)
        : type(ConstraintType::ABSOLUTE), absolute(absolute) {
    }

    inline Constraint(const RelativeConstraint& relative)
        : type(ConstraintType::RELATIVE), relative(relative) {
    }

    inline Constraint(const AspectConstraint& aspect)
        : type(ConstraintType::ASPECT), asp(aspect) {
    }

    inline Constraint(const FlexConstraint& flex)
        : type(ConstraintType::FLEX), flex(flex) {
    }

    inline Constraint(const CenterConstraint& center)
        : type(ConstraintType::CENTER) {
    }

    inline Constraint(const FitToContentConstraint& fitContent)
        : type(ConstraintType::FIT_TO_CONTENT) {
    }

    inline Constraint& operator=(const AbsoluteConstraint& absolute) {
        type = ConstraintType::ABSOLUTE;
        this->absolute = absolute;
        return *this;
    }

    inline Constraint& operator=(const RelativeConstraint& relative) {
        type = ConstraintType::RELATIVE;
        this->relative = relative;
        return *this;
    }

    inline Constraint& operator=(const AspectConstraint& aspect) {
        type = ConstraintType::ASPECT;
        asp = aspect;
        return *this;
    }

    inline Constraint& operator=(const FlexConstraint& aspect) {
        type = ConstraintType::FLEX;
        flex = flex;
        return *this;
    }

    inline Constraint& operator=(const CenterConstraint& center) {
        type = ConstraintType::CENTER;
        return *this;
    }

    inline Constraint& operator=(const FitToContentConstraint& fit) {
        type = ConstraintType::FIT_TO_CONTENT;
        return *this;
    }

    ConstraintType getType() const {
        return type;
    }

    template<typename T>
    T& getValue();

    template<typename T>
    const T& getValue() const;

};

struct Constraints {
    Constraint x;
    Constraint y;
    Constraint width;
    Constraint height;

    inline Constraints()
        : x(CenterConstraint()),
          y(CenterConstraint()),
          width(RelativeConstraint(1.0f)),
          height(RelativeConstraint(1.0f)) {
    }

    virtual bool valid() const;
};
