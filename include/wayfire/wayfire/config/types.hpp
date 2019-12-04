#pragma once

#include <wayfire/config/option-types.hpp>
#include <glm/vec4.hpp>
#include <memory>

namespace wf
{
namespace option_type
{
/**
 * To create an option of a given type, from_string must be specialized for
 * parsing the type.
 *
 * @param string The string representation of the value.
 * @return The parsed value, if the string was valid.
 */
template<class Type> std::experimental::optional<Type> from_string(
    const std::string& string);

/**
 * To create an option of a given type, to_string must be specialized for
 * converting the type to string.
 * @return The string representation of a value.
 *   It is expected that from_string(to_string(value)) == value.
 */
template<class Type> std::string to_string(const Type& value);

/**
 * Construct an integer wrapper from the given string, which needs to represent
 * a valid signed 32-bit integer in decimal system.
 */
template<> std::experimental::optional<int>
    from_string<int>(const std::string&);

/**
 * Construct an double wrapper from the given string, which needs to represent
 * a valid signed 64-bit floating point number.
 */
template<> std::experimental::optional<double>
    from_string<double>(const std::string&);

/**
 * Construct an string wrapper from the given string.
 * The string should not contain newline characters.
 */
template<> std::experimental::optional<std::string>
    from_string<std::string>(const std::string&);

/**
 * Convert the given integer to a string.
 */
template<> std::string
    to_string<int>(const int& value);

/**
 * Convert the given double to a string.
 */
template<> std::string
    to_string<double>(const double& value);

/**
 * Convert the given string to a string.
 */
template<> std::string
    to_string<std::string>(const std::string& value);
}

/**
 * Represents a color in RGBA format.
 */
struct color_t
{
  public:
    /** Initialize a black transparent color (default) */
    color_t();

    /**
     * Initialize a new color value with the given values
     * Values will be clamped to the [0, 1] range.
     */
    color_t(double r, double g, double b, double a);

    /**
     * Initialize a new color value with the given values.
     * Values will be clamped to the [0, 1] range.
     */
    explicit color_t(const glm::vec4& value);

    /**
     * Compare colors channel-for-channel.
     * Comparisons use a small epsilon 1e-6.
     */
    bool operator == (const color_t& other) const;

    /** Red channel value */
    double r;
    /** Green channel value */
    double g;
    /** Blue channel value */
    double b;
    /** Alpha channel value */
    double a;
};

namespace option_type
{
/**
 * Create a new color value from the given hex string, format is either
 * #RRGGBBAA or #RGBA.
 */
template<> std::experimental::optional<color_t>
    from_string(const std::string& value);

/** Convert the color to its hex string representation. */
template<> std::string to_string(const color_t& value);
}

/**
 * A list of valid modifiers.
 * The enumerations values are the same as the ones in wlroots.
 */
enum keyboard_modifier_t
{
    /* Shift modifier, <shift> */
	KEYBOARD_MODIFIER_SHIFT = 1,
    /* Control modifier, <ctrl> */
	KEYBOARD_MODIFIER_CTRL = 4,
    /* Alt modifier, <alt> */
	KEYBOARD_MODIFIER_ALT = 8,
    /* Windows/Mac logo modifier, <super> */
	KEYBOARD_MODIFIER_LOGO = 64,
};

/**
 * Represents a single keyboard shortcut.
 */
struct keybinding_t
{
  public:
    /**
     * Construct a new keybinding with the given modifier and key.
     */
    keybinding_t(uint32_t modifier, uint32_t keyval);

    /* Check whether two keybindings refer to the same shortcut */
    bool operator == (const keybinding_t& other) const;

    /** @return The modifiers of the keybinding */
    uint32_t get_modifiers() const;
    /** @return The key of the keybinding */
    uint32_t get_key() const;

  private:
    /** The modifier mask of this keybinding */
    uint32_t mod;
    /** The key of this keybinding */
    uint32_t keyval;
};

namespace option_type
{
/**
 * Construct a new keybinding from the given string description.
 * Format is <modifier1> .. <modifierN> KEY_<keyname>, where whitespace
 * characters between the different modifiers and KEY_* are ignored.
 *
 * For a list of available modifieres, see @keyboard_modifier_t.
 *
 * The KEY_<keyname> is derived from evdev, and possible names are
 * enumerated in linux/input-event-codes.h
 *
 * For example, "<super> <alt> KEY_E" represents pressing the Logo, Alt and
 * E keys together.
 *
 * Special cases are "none" and "disabled", which result in modifiers and
 * key 0.
 */
template<> std::experimental::optional<keybinding_t> from_string(
    const std::string& description);

/** Represent the keybinding as a string. */
template<> std::string to_string(const keybinding_t& value);
}

/**
 * Represents a single button shortcut (pressing a mouse button while holding
 * modifiers).
 */
struct buttonbinding_t
{
  public:
    /**
     * Construct a new buttonbinding with the given modifier and button.
     */
    buttonbinding_t(uint32_t modifier, uint32_t button);

    /* Check whether two keybindings refer to the same shortcut */
    bool operator == (const buttonbinding_t& other) const;

    /** @return The modifiers of the buttonbinding */
    uint32_t get_modifiers() const;
    /** @return The button of the buttonbinding */
    uint32_t get_button() const;

  private:
    /** The modifier mask of this keybinding */
    uint32_t mod;
    /** The key of this keybinding */
    uint32_t button;
};

namespace option_type
{
/**
 * Construct a new buttonbinding from the given description.
 * The format is the same as a keybinding, however instead of KEY_* values,
 * the buttons are prefixed with BTN_*
 *
 * Special case are descriptions "none" and "disable", which result in
 * mod = button = 0
 */
template<> std::experimental::optional<buttonbinding_t> from_string(
    const std::string& description);

/** Represent the buttonbinding as a string. */
template<> std::string to_string(const buttonbinding_t& value);
}

/**
 * The different types of available gestures.
 */
enum touch_gesture_type_t
{
    /* Invalid gesture */
    GESTURE_TYPE_NONE       = 0,
    /* Swipe gesture, i.e moving in one direction */
    GESTURE_TYPE_SWIPE      = 1,
    /* Edge swipe, which is a swipe originating from the edge of the screen */
    GESTURE_TYPE_EDGE_SWIPE = 2,
    /* Pinch gesture, multiple touch points coming closer or farther apart
     * from the center */
    GESTURE_TYPE_PINCH      = 3,
};

enum touch_gesture_direction_t
{
    /* Swipe-specific */
    GESTURE_DIRECTION_LEFT  = (1 << 0),
    GESTURE_DIRECTION_RIGHT = (1 << 1),
    GESTURE_DIRECTION_UP    = (1 << 2),
    GESTURE_DIRECTION_DOWN  = (1 << 3),
    /* Pinch-specific */
    GESTURE_DIRECTION_IN    = (1 << 4),
    GESTURE_DIRECTION_OUT   = (1 << 5),
};

/**
 * Represents a touch gesture.
 *
 * A touch gesture has a type, direction and finger count.
 * Finger count can be arbitrary, although Wayfire supports only gestures
 * with finger count >= 3 currently.
 *
 * Direction can be either one of of @touch_gesture_direction_t or, in case of
 * the swipe gestures, it can be a bitwise OR of two non-opposing directions.
 */
struct touchgesture_t
{
    /**
     * Construct a new touchgesture_t with the given type, direction and finger
     * count. Invalid combinations result in an invalid gesture with type NONE.
     */
    touchgesture_t(touch_gesture_type_t type, uint32_t direction,
        int finger_count);

    /** @return The type of the gesture */
    touch_gesture_type_t get_type() const;

    /** @return The finger count of the gesture, if valid. Undefined otherwise */
    int get_finger_count() const;

    /** @return The direction of the gesture, if valid. Undefined otherwise */
    uint32_t get_direction() const;

    /**
     * Check whether two bindings are equal.
     * Beware that a binding might be only partially set, i.e it might not have
     * a direction. In this case, the direction acts as a wildcard, so the
     * touchgesture_t matches any touchgesture_t of the same type with the same
     * finger count
     */
    bool operator == (const touchgesture_t& other) const;

  private:
    /** Type of the gesture */
    touch_gesture_type_t type;
    /** Direction of the gesture */
    uint32_t direction;
    /** Number of fingers of the gesture */
    int finger_count;
};

namespace option_type
{
/**
 * Construct a new touchgesture_t with the type, direction and finger count
 * indicated in the description.
 *
 * Format:
 * 1. pinch [in|out] <fingercount>
 * 2. [edge-]swipe up|down|left|right <fingercount>
 * 3. [edge-]swipe up-left|right-down|... <fingercount>
 * 4. disable | none
 */
template<> std::experimental::optional<touchgesture_t> from_string(
    const std::string& description);

/** Represent the touch gesture as a string. */
template<> std::string to_string(const touchgesture_t& value);
}

/**
 * Represents a binding which can be activated via multiple actions -
 * keybindings, buttonbindings and touch gestures.
 */
struct activatorbinding_t
{
  public:
    /**
     * Initialize an empty activator binding, i.e one which cannot be activated
     * in any way.
     */
    activatorbinding_t();
    ~activatorbinding_t();

    /* Copy constructor */
    activatorbinding_t(const activatorbinding_t& other);
    /* Copy assignment */
    activatorbinding_t& operator = (const activatorbinding_t& other);

    /** @return true if the activator is activated by the given keybinding. */
    bool has_match(const keybinding_t& key) const;

    /** @return true if the activator is activated by the given buttonbinding. */
    bool has_match(const buttonbinding_t& button) const;

    /** @return true if the activator is activated by the given gesture. */
    bool has_match(const touchgesture_t& gesture) const;

    /**
     * Check equality of two activator bindings.
     *
     * @return true if the two activator bindings are activated by the exact
     *  same bindings, false otherwise.
     */
    bool operator == (const activatorbinding_t& other) const;

  public:
    struct impl;
    std::unique_ptr<impl> priv;
};

namespace option_type
{
/**
 * Create an activator string from the given string description.
 * The string consists of valid descriptions of keybindings, buttonbindings
 * and touch gestures, separated by a single '|' sign.
 */
template<> std::experimental::optional<activatorbinding_t> from_string(
    const std::string& string);

/** Represent the activator binding as a string. */
template<> std::string to_string(const activatorbinding_t& value);
}

}
