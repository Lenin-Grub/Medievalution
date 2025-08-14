#pragma once

/**
 * @brief A utility class providing common mathematical operations.
 */
class Math
{
public:
    /**
     * @brief Converts a value to a percentage of another value.
     *
     * @tparam T1 The type of the base value.
     * @tparam T2 The type of the percentage.
     * @param first The base value.
     * @param percentage The percentage to apply.
     * @return The calculated percentage of the base value as a float.
     */
    template <typename T1, typename T2>
    static float convertToPercentage(T1 first, T2 percentage);

    /**
     * @brief Calculates the center position relative to given dimensions.
     *
     * @tparam T1 The type of the first dimension (e.g., width).
     * @tparam T2 The type of the second dimension (e.g., height).
     * @param first The first dimension.
     * @param second The second dimension.
     * @return A vector representing the center point.
     */
    template <typename T1, typename T2>
    static sf::Vector2f setCentre(T1 first, T2 second);

    /**
     * @brief Maps a value from its range to a percentage [0..100].
     *
     * @tparam T The numeric type of the value and range bounds.
     * @param value The value to convert to percentage.
     * @param range_min The minimum value of the range.
     * @param range_max The maximum value of the range.
     * @return The percentage representation of the value within the range.
     */
    template <typename T>
    static T toPercent(T value, T range_min, T range_max);

    /**
     * @brief Maps a percentage [0..100] back to a value in the given range.
     *
     * @tparam T The numeric type of the percentage and range bounds.
     * @param percentage The percentage to convert (clamped between 0 and 100).
     * @param range_min The minimum value of the target range.
     * @param range_max The maximum value of the target range.
     * @return The value corresponding to the percentage within the range.
     */
    template <typename T>
    static T fromPercent(T percentage, T range_min, T range_max);

    /**
     * @brief Normalizes a 2D vector to unit length.
     *
     * If the vector's length is close to zero, returns a default unit vector (1, 0).
     *
     * @param vector The vector to normalize.
     * @return The normalized vector, or (1, 0) if the input vector is near-zero.
     */
    static sf::Vector2f normalizeVector(const sf::Vector2f& vector);

    /**
     * @brief Calculates a perpendicular vector (rotated 90 degrees counter-clockwise).
     *
     * @param vector The input vector.
     * @return A vector perpendicular to the input.
     */
    static sf::Vector2f perpendicular(const sf::Vector2f& vector);

    /**
     * @brief Multiplies a vector by a scalar value.
     *
     * @param vector The vector to multiply.
     * @param scalar The scalar multiplier.
     * @return The resulting vector after multiplication.
     */
    static sf::Vector2f multiply(const sf::Vector2f& vector, float scalar);

    /**
     * @brief Divides a vector by a scalar value.
     *
     * @param vector The vector to divide.
     * @param scalar The scalar divisor.
     * @return The resulting vector after division.
     */
    static sf::Vector2f divide(const sf::Vector2f& vector, float scalar);

    /**
     * @brief Calculates the length (magnitude) of a 2D vector.
     *
     * @param vector The input vector.
     * @return The Euclidean length of the vector.
     */
    static float length(const sf::Vector2f& vector);
};

#include "Math.inl"