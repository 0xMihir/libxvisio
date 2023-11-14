//
// Created by Mihir Patil on 11/5/23.
//

#include "pose.h"

namespace xv {
    Vector4 Pose::matrixToQuaternion(const Matrix3&matrix) {
        Vector4 quaternion;

        double trace = matrix[0][0] + matrix[1][1] + matrix[2][2];

        if (trace > 0) {
            const double scale = sqrt(trace + 1.0) * 2.0;
            quaternion[0] = 0.25 * scale;
            quaternion[1] = (matrix[2][1] - matrix[1][2]) / scale;
            quaternion[2] = (matrix[0][2] - matrix[2][0]) / scale;
            quaternion[3] = (matrix[1][0] - matrix[0][1]) / scale;
        }
        else if (matrix[0][0] > matrix[1][1] && matrix[0][0] > matrix[2][2]) {
            const double scale = sqrt(1.0 + matrix[0][0] - matrix[1][1] - matrix[2][2]) * 2.0;
            quaternion[0] = (matrix[2][1] - matrix[1][2]) / scale;
            quaternion[1] = 0.25 * scale;
            quaternion[2] = (matrix[0][1] + matrix[1][0]) / scale;
            quaternion[3] = (matrix[0][2] + matrix[2][0]) / scale;
        }
        else if (matrix[1][1] > matrix[2][2]) {
            const double scale = sqrt(1.0 + matrix[1][1] - matrix[0][0] - matrix[2][2]) * 2.0;
            quaternion[0] = (matrix[0][2] - matrix[2][0]) / scale;
            quaternion[1] = (matrix[0][1] + matrix[1][0]) / scale;
            quaternion[2] = 0.25 * scale;
            quaternion[3] = (matrix[1][2] + matrix[2][1]) / scale;
        }
        else {
            const double scale = sqrt(1.0 + matrix[2][2] - matrix[0][0] - matrix[1][1]) * 2.0;
            quaternion[0] = (matrix[1][0] - matrix[0][1]) / scale;
            quaternion[1] = (matrix[0][2] + matrix[2][0]) / scale;
            quaternion[2] = (matrix[1][2] + matrix[2][1]) / scale;
            quaternion[3] = 0.25 * scale;
        }
        return quaternion;
    }
}
