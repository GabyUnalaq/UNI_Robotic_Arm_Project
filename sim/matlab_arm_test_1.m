close all
clear all
clc

%%
l1 = 4;
l2 = 3;
l3 = 2;
theta1 = linspace(0, 150, 15);
theta2 = linspace(0, 150, 15);
theta3 = linspace(0, 150, 15);
%%

for i = 1:length(theta1)
    THETA1 = theta1(i);
    for j = 1:length(theta2)
        THETA2 = theta2(j);
        
        x0 = 0;
        y0 = 0;
        
        x1 = l1*cosd(THETA1);
        y1 = l1*sind(THETA1);
        
        x2 = x1+l2*cosd(THETA2);
        y2 = y1+l2*sind(THETA2);
        
        for k = 1:length(theta3)
            THETA3 = theta3(k);
            
            x3 = x2+l3*cosd(THETA3);
            y3= y2+l3*sind(THETA3);
            plot([x0 x1], [y0 y1], [x1 x2], [y1 y2], [x2 x3], [y2 y3] ,'linewidth', 2 )
            axis([-9 10 0 10]);
            pause(0.01)
        end
    end
end
      

        
