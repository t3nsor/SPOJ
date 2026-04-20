// 2026-04-20
// The hardest part of this problem is understanding the problem statement.
// First, what exactly does the value l mean?  I guess it must be the radius of
// the circle traced out by the *centre* of the ball (not the radius of the
// circle traced out by, say, the point on the ball furthest away from the axis
// of the cone).  That would fit best with the constraint l > R (because if
// l < R then the ball would not touch the cone at a single point).  Second,
// what does it mean for the ball to be "solid"?  It turns out that the answer
// depends on the ball's moment of inertia, so we will need to assume that the
// density is uniform (for which I = (2/5)MR^2, M being the mass of the ball).
// Third, is the speed limited by the coefficient of friction?  Since the
// coefficient of friction is not given, we will assume that it is effectively
// infinite.  (Note that an infinite coefficient of kinetic friction makes
// slipping impossible, but an infinite coefficient of static friction does not
// prevent rolling.)  And finally, we will see that there is in fact no upper
// limit for the speed of the ball, so the problem is really asking about the
// limit as that speed goes to infinity.
//
// The hard part of the problem has to do with describing the kinematics, so we
// will start here.  We are told that the ball travels in a horizontal circle,
// but we aren't explicitly told that the linear speed of the ball is constant.
// We will prove later that it must be.  Initially, we will analyze the system
// in a frame that rotates around the axis of the cone such that the centre of
// the ball always appears stationary.  Without loss of generality, we choose
// the x-axis in this frame so that the centre of the ball sits at the point
// (l, 0, h).  (Note that the apex of the cone is at z-coordinate 0, and h can
// be calculated using simple trigonometry, but its value is not needed.)
//
// We are told that the ball rolls without slipping.  This implies that the
// point of contact on the ball has an instantaneous velocity due to the ball's
// rotation that matches the instantaneous velocity of the point of contact on
// the cone, which rotates around the cone's axis (since we are in the rotating
// frame).  To wit, the distance of the point of contact from the cone's axis is
// l + R cos u, so its linear velocity is -ω(t)(l + R cos u)ŷ, where ω(t) is the
// current rate at which our frame rotates relative to the lab frame.
//
// For the point of contact on the ball to move purely in the y direction, the
// axis of rotation must lie in the xz plane.  Let the angular velocity of the
// ball be (ω_{r,1}(t), 0, ω_{r,3}(t)).  The linear velocity of the contact
// point is then given by the sum of contributions from the two components of
// the angular velocity,
//   v_{r,c}(t) = R (ω_{r,1}(t) sin u + ω_{r,3}(t) cos u) ŷ
// Therefore
//   R(ω_{r,1}(t) sin u + ω_{r,3}(t) cos u) = -ω(t)(l + R cos u).
//
// We are also told that the contact points trace out a circle on the surface of
// the ball.  For that condition to be met, there must be an axis (call it the
// "contact-circle axis") that rotates with the ball and keeps a constant angle
// from the normal axis.  Since the contact-circle axis precesses around the
// angular velocity, it can keep a constant angle with the normal axis only if
// it precesses around the normal axis (i.e., the angular velocity is parallel
// to the normal axis) or if it stays still (i.e., the contact-circle axis is
// parallel to the angular velocity).  The former is ruled out by the no-slip
// condition, as the other component of the angular velocity cannot be zero
// unless ω = 0.  We conclude that the contact-circle axis is parallel to the
// angular velocity.  This implies that the axis of the angular velocity is also
// constant over time, since if it were to be changing over time then the
// contact-circle axis cannot simultaneously remain parallel to it while also
// precessing around it.
//
// Since the angular velocity vector is fixed in direction, and given the
// previously derived relationship between ω_{r,1}, ω_{r,3}, and ω, we now see
// that ω_{r,1}(t) and ω_{r,3}(t) can vary over time only by remaining
// proportional to ω(t) and to each other.  However, any such variation would
// violate conservation of energy and is thus forbidden.  Therefore, it must be
// that in the lab frame, the ball travels around the cone at a fixed linear
// speed (ω being the angular frequency of the revolution) and in the rotating
// frame, the angular velocity with which the ball spins is constant (as a
// vector).
//
// We now return to the lab frame.  At t = 0, as before, the centre of the ball
// is at (l, 0, h).  Call the angular velocity of the ball's rotation around the
// contact-circle axis (ω_1(t), ω_2(t), ω_3(t)) where ω_2(0) = 0.  Since the
// axis of rotation was fixed in the rotating frame, in the lab frame it
// precesses around the cone's axis with angular frequency ω.  That is,
//   ω_1(t) = ω_1(0) cos ωt
//   ω_2(t) = ω_1(0) sin ωt
//   ω_3(t) = ω_3(0)
// In particular, the angular acceleration at t = 0 is obtained by
// differentiating the above.
//
// Now we consider the linear dynamics.  The forces acting on the ball are
// gravity, an initially unknown normal force, and an initially unknown friction
// force whose direction we don't know.  These three forces combined must
// produce the centripetal force needed to keep the ball in its circular
// trajectory.  Using this fact, we can obtain the friction force as a function
// of ω.  (Note that by choosing units of mass and time appropriately, we can
// make the mass of the ball and the local gravitational acceleration both equal
// to 1.)
//
// Of the three forces acting on the ball, the only one that exerts a torque
// around the centre of the ball is the friction force.  By setting this torque
// equal to the angular acceleration of the ball's rotation at t = 0 times the
// moment of inertia, we solve for ω_1(0) in terms of ω.  Note that at t = 0
// (where the axes of the lab frame coincide with those of the rotating frame),
// ω_{r,1} = ω_1.  Also, we have already determined the relationship between
// ω_{r,1} and ω_{r,3}, so ω_{r,3} is also determined in terms of ω.
//
// Notably, these equations have solutions for all values of ω > 0.  This
// implies that "as fast as possible" refers to the limit as ω goes to infinity.
// In that limit, the direction of the angular velocity in the rotating frame,
// (ω_{r,1}, 0, ω_{r,3}), approaches a fixed direction.  Finally, the radius of
// the contact circle is just R sin θ, where θ is the angle between the angular
// velocity and the vector (R cos θ, 0, -R sin θ) from the centre of the ball
// to the contact point.
//
// Some additional remarks can be found here: https://www.physics.harvard.edu/sites/g/files/omnuum6476/files/physics/files/sol33.pdf

#include <math.h>
#include <stdio.h>
constexpr double pi = acos(-1.0);
int main() {
    double l, R, u;
    while (scanf("%lf %lf %lf", &l, &R, &u) == 3) {
        u = pi*u/180;
        // direction of the angular velocity in the rotating frame
        const double wx = 2.5*l*sin(u)*cos(u);
        const double wz = -l - cos(u) - 2.5*l*sin(u)*sin(u);
        // cosine of the angle θ described above
        const double c = (wx*cos(u) - wz*sin(u))/hypot(wx, wz);
        printf("%d\n", int(R * sqrt(1 - c*c) + 0.5));
    }
}
