//
// Created by Mihir Patil on 10/26/23.
//

#ifndef XVISIO_POSE_H
#define XVISIO_POSE_H

#include <array>

namespace xv {
    using Matrix3 = std::array<std::array<double, 3>, 3>;
    using Vector3 = std::array<double, 3>;
    using Vector4 = std::array<double, 4>;

    struct Pose {
        Pose(const Vector3&position, const Matrix3&rotationMatrix, const int64_t timestamp) : position(position),
            matrix(rotationMatrix),
            timestamp(timestamp),
            quaternion(matrixToQuaternion(rotationMatrix)) {
        }

        static Vector4 matrixToQuaternion(const Matrix3&matrix);

        Vector3 position;
        Matrix3 matrix;
        Vector4 quaternion;
        int64_t timestamp;
    };
}


#endif //XVISIO_POSE_H
