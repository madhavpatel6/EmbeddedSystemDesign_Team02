clear;
close all;

path = [0.1    0.1;
        1.9     0.1;
        1.9     0.25;
        0.1     0.25;
        0.1     0.40;
        1.9     0.40;
        1.9     0.55;
        0.1     0.55;
        0.1     0.70;
        1.9     0.70;
        1.9     0.85;
        0.1     0.85;
        0.1     1.00;
        1.9     1.00];

% Set the current location and the goal location of the robot as defined by the path
robotCurrentLocation = path(1,:);
robotGoal = path(end,:);

initialOrientation = 0;

robotCurrentPose = [robotCurrentLocation initialOrientation];

robotRadius = 0.07;

robot = ExampleHelperRobotSimulator('emptyMap', 2);
robot.enableLaser(false);
robot.setRobotSize(robotRadius);
robot.showTrajectory(true);
robot.setRobotPose(robotCurrentPose);

plot(path(:,1), path(:,2),'k--d');
grid on;
xlim([0 2.0]);
ylim([0 2.0]);

controller = robotics.PurePursuit;
controller.Waypoints = path;
controller.DesiredLinearVelocity = 0.3;
controller.MaxAngularVelocity = 100;
controller.LookaheadDistance = 0.05;

goalRadius = 0.1;
distanceToGoal = norm(robotCurrentLocation - robotGoal);

controlRate = robotics.Rate(20);

while( distanceToGoal > goalRadius )

    % Compute the controller outputs, i.e., the inputs to the robot
    [v, omega] = step(controller, robot.getRobotPose)

    % Simulate the robot using the controller outputs.
    drive(robot, v, omega);

    % Extract current location information ([X,Y]) from the current pose of
    % the robot
    robotCurrentPose = robot.getRobotPose;

    % Re-compute the distance to the goal
    distanceToGoal = norm(robotCurrentPose(1:2) - robotGoal);

    waitfor(controlRate);

end

delete(robot);