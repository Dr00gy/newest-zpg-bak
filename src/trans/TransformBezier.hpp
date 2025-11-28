#pragma once
#include "Transform.hpp"
#include <glm/gtx/string_cast.hpp>
#include <algorithm>
#include <vector>

class TransformBezier : public Transform {
public:
    TransformBezier(const glm::vec3& p0, const glm::vec3& p1, 
                    const glm::vec3& p2, const glm::vec3& p3, 
                    float t = 0.0f)
        : param(t) {
        ctrlPts.push_back(p0);
        ctrlPts.push_back(p1);
        ctrlPts.push_back(p2);
        ctrlPts.push_back(p3);
        A = glm::mat4(glm::vec4(-1.0, 3.0, -3.0, 1.0),
                      glm::vec4(3.0, -6.0, 3.0, 0.0),
                      glm::vec4(-3.0, 3.0, 0.0, 0.0),
                      glm::vec4(1.0, 0.0, 0.0, 0.0));
    }

    TransformBezier(const std::vector<glm::vec3>& pts, float t = 0.0f)
        : ctrlPts(pts), param(t) {
        A = glm::mat4(glm::vec4(-1.0, 3.0, -3.0, 1.0),
                      glm::vec4(3.0, -6.0, 3.0, 0.0),
                      glm::vec4(-3.0, 3.0, 0.0, 0.0),
                      glm::vec4(1.0, 0.0, 0.0, 0.0));
    }

    void setParam(float t) {
        param = glm::clamp(t, 0.0f, 1.0f);
    }

    float getParam() const {
        return param;
    }

    void setCtrlPts(const std::vector<glm::vec3>& pts) {
        ctrlPts = pts;
    }

    std::vector<glm::vec3> getCtrlPts() const {
        return ctrlPts;
    }

    glm::vec3 getPositionOnCurve() const {
        if (ctrlPts.size() < 4) {
            return glm::vec3(0.0f);
        }
        
        int numSegments = (ctrlPts.size() - 1) / 3;
        if (numSegments < 1) numSegments = 1;
        float segmentParam = param * numSegments;
        int segmentIndex = static_cast<int>(segmentParam);
        
        if (segmentIndex >= numSegments) {
            segmentIndex = numSegments - 1;
            segmentParam = 1.0f;
        } else {
            segmentParam = segmentParam - segmentIndex;
        }
        
        int baseIndex = segmentIndex * 3;
        if (ctrlPts.size() == 4) {
            baseIndex = 0;
        }
        if (baseIndex + 3 >= static_cast<int>(ctrlPts.size())) {
            baseIndex = ctrlPts.size() - 4;
        }
        
        glm::mat4x3 B = glm::mat4x3(
            ctrlPts[baseIndex],
            ctrlPts[baseIndex + 1],
            ctrlPts[baseIndex + 2],
            ctrlPts[baseIndex + 3]
        );
        
        float t = segmentParam;
        glm::vec4 T = glm::vec4(t * t * t, t * t, t, 1.0f);
        
        glm::vec3 position = T * A * glm::transpose(B);
        return position;
    }
    
    // P'(t) = [3t², 2t, 1, 0] * A * B
    glm::vec3 getTangentOnCurve() const {
        if (ctrlPts.size() < 4) {
            return glm::vec3(0.0f, 0.0f, 1.0f);
        }
        
        int numSegments = (ctrlPts.size() - 1) / 3;
        if (numSegments < 1) numSegments = 1;
        float segmentParam = param * numSegments;
        int segmentIndex = static_cast<int>(segmentParam);
        
        if (segmentIndex >= numSegments) {
            segmentIndex = numSegments - 1;
            segmentParam = 1.0f;
        } else {
            segmentParam = segmentParam - segmentIndex;
        }
        
        int baseIndex = segmentIndex * 3;
        if (ctrlPts.size() == 4) {
            baseIndex = 0;
        }
        if (baseIndex + 3 >= static_cast<int>(ctrlPts.size())) {
            baseIndex = ctrlPts.size() - 4;
        }
        
        glm::mat4x3 B = glm::mat4x3(
            ctrlPts[baseIndex],
            ctrlPts[baseIndex + 1],
            ctrlPts[baseIndex + 2],
            ctrlPts[baseIndex + 3]
        );
        
        // P'(t) = T' * A * B, T' = [3t², 2t, 1, 0]
        float t = segmentParam;
        glm::vec4 T_prime = glm::vec4(3.0f * t * t, 2.0f * t, 1.0f, 0.0f);
        glm::vec3 tangent = T_prime * A * glm::transpose(B);
        
        if (glm::length(tangent) > 0.0001f) {
            return glm::normalize(tangent);
        }
        return glm::vec3(0.0f, 0.0f, 1.0f);
    }

    glm::mat4 getMatrix() const override {
        glm::vec3 position = getPositionOnCurve();
        glm::vec3 tangent = getTangentOnCurve();
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 forward = tangent;
        glm::vec3 right = glm::normalize(glm::cross(worldUp, forward));
        
        if (glm::length(right) < 0.001f) {
            right = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        
        glm::vec3 up = glm::normalize(glm::cross(forward, right));
        glm::mat4 rotation = glm::mat4(1.0f);
        rotation[0] = glm::vec4(right, 0.0f);
        rotation[1] = glm::vec4(up, 0.0f);
        rotation[2] = glm::vec4(forward, 0.0f);
        
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        return translation * rotation;
    }

private:
    std::vector<glm::vec3> ctrlPts;
    float param;
    glm::mat4 A;
};