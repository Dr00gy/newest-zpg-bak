#pragma once
#include "Transform.hpp"
#include <vector>

class TransformBezier : public Transform {
public:
    TransformBezier(const std::vector<glm::vec3>& points, float t = 0.0f)
        : controlPoints(points), param(t) {
        basisMatrix = glm::mat4(
            glm::vec4(-1.0,  3.0, -3.0, 1.0),
            glm::vec4( 3.0, -6.0,  3.0, 0.0),
            glm::vec4(-3.0,  3.0,  0.0, 0.0),
            glm::vec4( 1.0,  0.0,  0.0, 0.0)
        );
    }

    void setParam(float t) {
        param = glm::clamp(t, 0.0f, 1.0f);
    }

    glm::mat4 getMatrix() const override {
        glm::vec3 position = calcPos();
        glm::vec3 tangent = calcTan();
        
        glm::vec3 forward = glm::normalize(tangent);
        glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
        glm::vec3 up = glm::normalize(glm::cross(forward, right));
        
        glm::mat4 transform = glm::mat4(1.0f);
        transform[0] = glm::vec4(right, 0);
        transform[1] = glm::vec4(up, 0);
        transform[2] = glm::vec4(forward, 0);
        transform[3] = glm::vec4(position, 1);
        
        return transform;
    }

private:
    std::vector<glm::vec3> controlPoints;
    float param;
    glm::mat4 basisMatrix;
    
    glm::vec3 calcPos() const {
        if (controlPoints.size() < 4) return glm::vec3(0);
        
        int segIdx;
        float localT;
        getSegInfo(segIdx, localT);
        
        int baseIdx = segIdx * 3;
        glm::mat3x4 P = glm::mat3x4(
            controlPoints[baseIdx].x, controlPoints[baseIdx+1].x, 
            controlPoints[baseIdx+2].x, controlPoints[baseIdx+3].x,
            
            controlPoints[baseIdx].y, controlPoints[baseIdx+1].y,
            controlPoints[baseIdx+2].y, controlPoints[baseIdx+3].y,
            
            controlPoints[baseIdx].z, controlPoints[baseIdx+1].z,
            controlPoints[baseIdx+2].z, controlPoints[baseIdx+3].z 
        );
        
        glm::vec4 T = glm::vec4(
            localT * localT * localT,
            localT * localT,
            localT,
            1.0f
        );
        
        return T * basisMatrix * P; // left to right to get w and then x,y,z
    }
    
    glm::vec3 calcTan() const {
        if (controlPoints.size() < 4) return glm::vec3(0, 0, 1);
        
        int segIdx;
        float localT;
        getSegInfo(segIdx, localT);
        
        int baseIdx = segIdx * 3;
        glm::mat3x4 P = glm::mat3x4(
            controlPoints[baseIdx].x, controlPoints[baseIdx+1].x, 
            controlPoints[baseIdx+2].x, controlPoints[baseIdx+3].x,
            
            controlPoints[baseIdx].y, controlPoints[baseIdx+1].y,
            controlPoints[baseIdx+2].y, controlPoints[baseIdx+3].y,
            
            controlPoints[baseIdx].z, controlPoints[baseIdx+1].z,
            controlPoints[baseIdx+2].z, controlPoints[baseIdx+3].z 
        );
        
        glm::vec4 T_derivative = glm::vec4(
            3.0f * localT * localT,
            2.0f * localT,
            1.0f,
            0.0f 
        );
        
        glm::vec3 tangent = T_derivative * basisMatrix * P;
        return tangent;
    }
    
    // which curve segment and local t parameter
    void getSegInfo(int& segIdx, float& localT) const {
        int numSeggs = (controlPoints.size() - 1) / 3;
        if (numSeggs < 1) numSeggs = 1;
        
        float globalT = param * numSeggs;
        segIdx = static_cast<int>(globalT);
        
        if (segIdx >= numSeggs) {
            segIdx = numSeggs - 1;
            localT = 1.0f;
        } else {
            localT = globalT - segIdx;
        }
        
        int baseIdx = segIdx * 3;
        if (baseIdx + 3 >= static_cast<int>(controlPoints.size())) {
            baseIdx = controlPoints.size() - 4;
            segIdx = baseIdx / 3;
        }
    }
};