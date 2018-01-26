function [] = HW_K3()
    F = @(x, u) (u^2 - x + 0.5) / (x^2 + u + 1);
    a = 2; u0 = 0; N = 40;
    [ tout, yout ] = RungeKutta( '(y^2 - t + 0.5) / (t^2 + y + 1)',[0, a],u0,a/N );
    plot(tout, yout, 'Color', [0 0 0]);
    hold on;
    [tout, yout] = PredictorCorrector( F, a, u0, N );
    plot(tout, yout, 'LineStyle', '--');
    hold off;
    legend('PredictorCorrector', 'RungeKutta')
    xlabel('t');
    ylabel('y');
    format long;
    fig = figure;
    table = uitable(fig, 'Data', [tout yout], 'Position', [20 20 260 204]);
end

function [ tout, yout ] = PredictorCorrector( F, a, y0, N )
    epsilon = 0.001;
    h = a/N;
    P = @(yi, y1, y2, y3) yi + (h / 12) * (23*y1 - 16*y2 + 5*y3);
    C = @(yi, y1, y2, y3) yi + (h / 12) * ( 5*y1 +  8*y2 -   y3);
    % Prealloc for performance
    tout = zeros(N + 1, 1); tout(1) = 0;
    yout = zeros(N + 1, 1); yout(1) = y0;
    
    % Runge Kutta for first 2 elements
    for i = 2:3
        [tout(i), yout(i)] = RungeKuttaStep(F, tout(i-1), yout(i-1), h);
    end
    
    for i = 3:N
        % Predictor
        tout(i+1) = tout(i) + h;
        y1 = F(tout(i), yout(i));
        y2 = F(tout(i-1), yout(i-1));
        y3 = F(tout(i-2), yout(i-2));
        yout(i+1) = P(yout(i), y1, y2, y3);
        
        % Corrector
        for j = 1:4
            yj = yout(i+1);
            yout(i+1) = C(yout(i), F(tout(i+1), yj), y1, y2);
            if abs(yj - yout(i+1)) < epsilon
                break;
            end
        end
    end
end

% Runge Kutta - O(h^3)
function [ tout, yout ] = RungeKuttaStep( F, t, y, h )
    s1=feval(F,t,y);
    s2=feval(F,t+h/2,y+h*s1/2);
    s3=feval(F,t+h,y-h*s1+2*h*s2);
    tout=t+h;
    yout=y+(h/6)*(s1+4*s2+s3);
end

% Runge Kutta (O(h^4)) - previous homework
function [ tout, yout ] = RungeKutta( F,tspan,y0,h )
    F=inline(F,'t','y');
    t0=tspan(1);
    tfinal=tspan(2);
    t=t0;
    y=y0;
    tout=t;
    yout=y';
    while t<tfinal
        s1=feval(F,t,y);
        s2=feval(F,t+0.5*h,y+0.5*h*s1);
        s3=feval(F,t+0.5*h,y+0.5*h*s2);
        s4=feval(F,t+h,y+h*s3);
        y=y+(h/6)*(s1+2*s2+2*s3+s4);
        t=t+h;
        tout(end+1,1)=t;
        yout(end+1,:)=y';
    end
end