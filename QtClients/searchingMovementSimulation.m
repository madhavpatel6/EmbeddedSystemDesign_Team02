clear;
close all;

max_v = 0.4;
w = pi/4;

straightLookahead = 0.5;
turnLookahead = 0.1;

delay = 0.15;

distance = 0.15;

% Set the current location and the goal location of the robot as defined by the path
robotCurrentLocation = [0.1 0.1];
robotGoal = [2.0 2.0];

path = [robotCurrentLocation;
    robotCurrentLocation + [distance 0]];

initialOrientation = 0;
robotCurrentPose = [robotCurrentLocation initialOrientation];

robotRadius = 0.07;
robot = ExampleHelperRobotSimulator('borderMap', 25);
robot.enableLaser(false);
robot.setRobotSize(robotRadius);
robot.showTrajectory(true);
robot.setRobotPose(robotCurrentPose);
robotOdom = robot.getRobotOdom;
prevRobotOdom = [0 0 pi/2];

grid on;
pathPlot = plot(path(:,1), path(:,2),'k--d');
xlim([-0.2 2.2]);
ylim([-0.2 2.2]);

controller = robotics.PurePursuit;
controller.Waypoints = path;
controller.DesiredLinearVelocity = max_v;
controller.MaxAngularVelocity = w;
controller.LookaheadDistance = straightLookahead;

goalRadius = 0.1;
distanceToGoal = norm(robotCurrentLocation - robotGoal);

controlRate = robotics.Rate(10);

state = 'right';

while( distanceToGoal > goalRadius )
    switch state
        case 'right'
            % Compute the controller outputs, i.e., the inputs to the robot
            [v, omega] = step(controller,robot.getRobotPose);
            
            if abs((robotOdom(3)+0.0001) - prevRobotOdom(3)) < ((pi/2)-0.0001)
                v = 0;
            end
            
            % Simulate the robot using the controller outputs.
            drive(robot, v, omega);
            pause(delay);
%             waitfor(controlRate);
            
            temp = robot.getRobotPose;

            if (robotCurrentPose == temp)
                if (robotCurrentPose(3) < (10*pi/180) || robotCurrentPose(3) > (350*pi/180))
                    prevRobotOdom = robot.getRobotOdom;
                    temp = robot.getRobotPose;
                    path = [temp(1:2);
                    temp(1:2) + [-0.001 distance]];
                    controller.release();
                    controller = robotics.PurePursuit;
                    controller.Waypoints = path;
                    controller.DesiredLinearVelocity = max_v;
                    controller.MaxAngularVelocity = w;
                    controller.LookaheadDistance = turnLookahead;
                    delete(pathPlot);
                    pathPlot = plot(path(:,1), path(:,2),'k--d');
                    state = 'turnLeft';
                    continue;
                elseif (robotCurrentPose(3) < (190*pi/180) || robotCurrentPose(3) > (170*pi/180))
                    prevRobotOdom = robot.getRobotOdom;
                    temp = robot.getRobotPose;
                    path = [temp(1:2);
                    temp(1:2) + [0.001 distance]];
                    controller.release();
                    controller = robotics.PurePursuit;
                    controller.Waypoints = path;
                    controller.DesiredLinearVelocity = max_v;
                    controller.MaxAngularVelocity = w;
                    controller.LookaheadDistance = turnLookahead;
                    delete(pathPlot);
                    pathPlot = plot(path(:,1), path(:,2),'k--d');
                    state = 'turnRight';
                    continue;
                end
            end
            
            temp = robot.getRobotPose;
            path = [temp(1:2);
            temp(1:2) + [distance 0]];
            controller.release();
            controller = robotics.PurePursuit;
            controller.Waypoints = path;
            controller.DesiredLinearVelocity = max_v;
            controller.MaxAngularVelocity = w;
            controller.LookaheadDistance = straightLookahead;
            delete(pathPlot);
            pathPlot = plot(path(:,1), path(:,2),'k--d');
        case 'left'
            % Compute the controller outputs, i.e., the inputs to the robot
            [v, omega] = step(controller,robot.getRobotPose);
            
            if abs((robotOdom(3)+0.0001) - prevRobotOdom(3)) < ((pi/2)-0.0001)
                v = 0;
            end
            
            % Simulate the robot using the controller outputs.
            drive(robot, v, omega);
            pause(delay);
%             waitfor(controlRate);
            
            if (robotCurrentPose == robot.getRobotPose)
                if (robotCurrentPose(3) < (10*pi/180) || robotCurrentPose(3) > (350*pi/180))
                    prevRobotOdom = robot.getRobotOdom;
                    temp = robot.getRobotPose;
                    path = [temp(1:2);
                    temp(1:2) + [-0.001 distance]];
                    controller.release();
                    controller = robotics.PurePursuit;
                    controller.Waypoints = path;
                    controller.DesiredLinearVelocity = max_v;
                    controller.MaxAngularVelocity = w;
                    controller.LookaheadDistance = turnLookahead;
                    delete(pathPlot);
                    pathPlot = plot(path(:,1), path(:,2),'k--d');
                    state = 'turnLeft';
                    continue;
                elseif (robotCurrentPose(3) < (190*pi/180) || robotCurrentPose(3) > (170*pi/180))
                    prevRobotOdom = robot.getRobotOdom;
                    temp = robot.getRobotPose;
                    path = [temp(1:2);
                    temp(1:2) + [0.001 distance]];
                    controller.release();
                    controller = robotics.PurePursuit;
                    controller.Waypoints = path;
                    controller.DesiredLinearVelocity = max_v;
                    controller.MaxAngularVelocity = w;
                    controller.LookaheadDistance = turnLookahead;
                    delete(pathPlot);
                    pathPlot = plot(path(:,1), path(:,2),'k--d');
                    state = 'turnRight';
                    continue;
                end
            end
            
            temp = robot.getRobotPose;
            path = [temp(1:2);
            temp(1:2) + [-distance 0]];
            controller.release();
            controller = robotics.PurePursuit;
            controller.Waypoints = path;
            controller.DesiredLinearVelocity = max_v;
            controller.MaxAngularVelocity = w;
            controller.LookaheadDistance = straightLookahead;
            delete(pathPlot);
            pathPlot = plot(path(:,1), path(:,2),'k--d');
        case 'turnLeft'
            % Compute the controller outputs, i.e., the inputs to the robot
            [v, omega] = step(controller,robot.getRobotPose);
            
            if abs((robotOdom(3)+0.0001) - prevRobotOdom(3)) < ((pi/2)-0.0001)
                v = 0;
            end
            
            % Simulate the robot using the controller outputs.
            drive(robot, v, omega);
            pause(delay);
%             waitfor(controlRate);
            
            temp = robot.getRobotPose;
            
            if (norm(temp(1:2) - path(end,:)) < 0.05)
                    prevRobotOdom = robot.getRobotOdom;
                    temp = robot.getRobotPose;
                    path = [temp(1:2);
                    temp(1:2) + [-distance 0]];
                    controller.release();
                    controller = robotics.PurePursuit;
                    controller.Waypoints = path;
                    controller.DesiredLinearVelocity = max_v;
                    controller.MaxAngularVelocity = w;
                    controller.LookaheadDistance = straightLookahead;
                    delete(pathPlot);
                    pathPlot = plot(path(:,1), path(:,2),'k--d');
                    state = 'left';
                    continue;
            end
        case 'turnRight'
            % Compute the controller outputs, i.e., the inputs to the robot
            [v, omega] = step(controller,robot.getRobotPose);
            
            if abs((robotOdom(3)+0.0001) - prevRobotOdom(3)) < ((pi/2)-0.0001)
                v = 0;
            end
            
            % Simulate the robot using the controller outputs.
            drive(robot, v, omega);
            pause(delay);
%             waitfor(controlRate);
            
            temp = robot.getRobotPose;
            
            if (norm(temp(1:2) - path(end,:)) < 0.05)
                    prevRobotOdom = robot.getRobotOdom;
                    temp = robot.getRobotPose;
                    path = [temp(1:2);
                    temp(1:2) + [distance 0]];
                    controller.release();
                    controller = robotics.PurePursuit;
                    controller.Waypoints = path;
                    controller.DesiredLinearVelocity = max_v;
                    controller.MaxAngularVelocity = w;
                    controller.LookaheadDistance = straightLookahead;
                    delete(pathPlot);
                    pathPlot = plot(path(:,1), path(:,2),'k--d');
                    state = 'right';
                    continue;
            end
        otherwise
    end
    
    % Extract current location information ([X,Y]) from the current pose of
    % the robot
    robotCurrentPose = robot.getRobotPose;
    robotOdom = robot.getRobotOdom;
    
    if robotCurrentPose(3) < 0
        robotCurrentPose(3) = robotCurrentPose(3) + (2*pi);
    end
    
    if robotOdom(3) < 0
        robotOdom(3) = robotOdom(3) + (2*pi);
    end
    % Re-compute the distance to the goal
    distanceToGoal = norm(robotCurrentPose(1:2) - robotGoal);
end

delete(robot);