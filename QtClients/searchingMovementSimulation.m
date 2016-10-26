clear;
close all;

% Set the current location and the goal location of the robot as defined by the path
robotCurrentLocation = [0.1 0.1];
robotGoal = [1.9 1.9];

initialOrientation = 0;
robotCurrentPose = [robotCurrentLocation initialOrientation];

robotRadius = 0.07;

robot = ExampleHelperRobotSimulator('borderMap', 25);
robot.enableLaser(false);
robot.setRobotSize(robotRadius);
robot.showTrajectory(true);
robot.setRobotPose(robotCurrentPose);

grid on;
xlim([0 2.0]);
ylim([0 2.0]);

% controller = roboticss.PurePursuit;
% controller.Waypoints = path;
% controller.DesiredLinearVelocity = 0.3;
% controller.MaxAngularVelocity = 100;
% controller.LookaheadDistance = 0.05;

goalRadius = 0.1;
distanceToGoal = norm(robotCurrentLocation - robotGoal);

controlRate = robotics.Rate(20);

state = 'forward';

while( distanceToGoal > goalRadius )
    robotPreviousPose = robot.getRobotPose;
    switch state
        case 'forward'
            % Compute the controller outputs, i.e., the inputs to the robot
            % [v, omega] = step(controller, robot.getRobotPose)
            v = 0.3;
            omega = 0;
            
            % Simulate the robot using the controller outputs.
            drive(robot, v, omega);
            
            if (robotPreviousPose == robot.getRobotPose)
                if robotCurrentPose(3) == 0
                    state = 'turnLeft';
                end
            end
        case 'turnLeft'
        case 'turnRight'
        otherwise
    end
    
    % Extract current location information ([X,Y]) from the current pose of
    % the robot
    robotCurrentPose = robot.getRobotPose;

    % Re-compute the distance to the goal
    distanceToGoal = norm(robotCurrentPose(1:2) - robotGoal);
    
    waitfor(controlRate);
end

delete(robot);