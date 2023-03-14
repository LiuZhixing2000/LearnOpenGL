#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

// defines several possible options for camera movement
enum camera_movement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class camera_{
public:
    // camera attributes
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;
    // euler angles
    float yaw_;
    float pitch_;
    // camera options
    float movement_speed_;
    float mouse_sensitivity_;
    float zoom_;

    // construct with vectors
    camera_(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
    : front_(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed_(SPEED), mouse_sensitivity_(SENSITIVITY), zoom_(ZOOM){
        position_ = position;
        world_up_ = up;
        yaw_ = yaw;
        pitch_ = pitch;
        update_camera_vectors();
    }
    // construct with scalar values
    camera_(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch)
    : front_(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed_(SPEED), mouse_sensitivity_(SENSITIVITY), zoom_(ZOOM){
        position_ = glm::vec3(pos_x, pos_y, pos_z);
        world_up_ = glm::vec3(up_x, up_y, up_z);
        yaw_ = yaw;
        pitch_ = pitch;
        update_camera_vectors();
    }

    // return the view matrix
    glm::mat4 get_view_matrix(){
        return glm::lookAt(position_, position_ + front_, up_);
    }

    // process keyboard input
    void process_keyboard(camera_movement direction, float delta_time){
        float velocity = movement_speed_ * delta_time;
        if (direction == FORWARD)
            position_ += front_ * velocity;
        if (direction == BACKWARD)
            position_ -= front_ * velocity;
        if (direction == LEFT)
            position_ -= right_ * velocity;
        if (direction == RIGHT)
            position_ += right_ * velocity;
    }

    // process mouse input
    void process_mouse_movement(float xoffset, float yoffset, GLboolean constrain_pitch = true){
        xoffset *= mouse_sensitivity_;
        yoffset *= mouse_sensitivity_;

        yaw_ += xoffset;
        pitch_ += yoffset;
        
        if (constrain_pitch){
            pitch_ = std::min(pitch_, 89.0f);
            pitch_ = std::max(pitch_, -89.0f);
        }

        update_camera_vectors();
    }

    // process mouse scroll
    void process_mouse_scroll(float yoffset){
        zoom_ -= (float)yoffset;
        zoom_ = std::max(1.0f, zoom_);
        zoom_ = std::min(45.0f, zoom_);
    }
private:
    // calculate the front vector from the camera's Euler Angles
    void update_camera_vectors(){
        glm::vec3 front;
        front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front.y = sin(glm::radians(pitch_));
        front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front_ = glm::normalize(front);

        right_ = glm::normalize(glm::cross(front_, world_up_));
        up_ = glm::normalize(glm::cross(right_, front_));
    }
};