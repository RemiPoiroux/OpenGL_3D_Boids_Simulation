#include <glm/vec3.hpp>

class DirectionalLight
{
private:
    glm::vec3 m_direction;
    glm::vec3 m_color;

public:
    DirectionalLight(const glm::vec3& direction, const glm::vec3& color)
        : m_direction(direction), m_color(color) {}

    const glm::vec3& getDirection() const
    {
        return m_direction;
    }

    const glm::vec3& getColor() const
    {
        return m_color;
    }
};

class PointLight
{
private:
    glm::vec3 m_position;
    glm::vec3 m_color;
    float     m_intensity;

public:
    PointLight(const glm::vec3& position, float intensity, const glm::vec3& color)
        : m_position(position), m_color(color), m_intensity(intensity) {}

    const glm::vec3& getPosition() const
    {
        return m_position;
    }

    const glm::vec3& getColor() const
    {
        return m_color;
    }

    float getIntensity() const
    {
        return m_intensity;
    }

    void setIntensity(float newIntensity)
    {
        this->m_intensity = newIntensity;
    }
};

class Material
{
private:
    glm::vec3 m_kd;
    glm::vec3 m_ks;
    float     m_shininess;

public:
    Material(const glm::vec3& kd, const glm::vec3& ks, float shininess)
        : m_kd(kd), m_ks(ks), m_shininess(shininess)
    {}

    const glm::vec3& getKd() const
    {
        return m_kd;
    }

    const glm::vec3& getKs() const
    {
        return m_ks;
    }

    float getShininess() const
    {
        return m_shininess;
    }
};
