function [] = HW_K6()
    F = @(x1, x2) (x1*sin(3*x2)*(-cos(3*x2)));
    l = 1;
    
    h = 0.05;
    % Run Jacobi's Method for 3 grids
    [xout, yout] = JacobisMethod(F, l, h);
    [~, yout2] = JacobisMethod(F, l, h/2);
    [xout4, yout4] = JacobisMethod(F, l, h/4);
    
    % Calculate practical rate of convergence
    rate = zeros(length(yout(:, 1)), length(yout(1, :)));
    for i = 1:length(rate(:, 1))
        for j = 1:length(rate(1, :))
            rate(i, j)=log2(abs((yout(i, j) - yout2(i*2 - 1, j*2 - 1))/(yout2(i*2 - 1, j*2 - 1) - yout4(i*4 - 3, j*4 - 3))));
        end
    end
    
    % Plot rate of convergence
    [xi, yi] = meshgrid(xout(2:end-1,1), xout(2:end-1,1));
    surf(xi, yi, rate(2:end-1,2:end-1));
    title(sprintf('Rate of convergence h = %.2f', h));
    xlabel('x1');
    ylabel('x2');
    zlabel('Rate');
    % Plot function graph]
    % [xi, yi] = meshgrid(xout4(:,1), xout4(:,1));
    % surf(xi, yi, yout4(:,:));
    % title(sprintf('Yij for h = %.2f', h));
    % xlabel('x1');
    % ylabel('x2');
    % zlabel('Y');
end

function [xout, yout] = JacobisMethod(F, l, h)
    N = round(l/h);
    epsilon = 10^-8;
    
    yold = zeros(N+1, N+1);
    ynew = zeros(N+1, N+1);
    xout = zeros(N+1);
    
    for i = 2:N+1
        xout(i) = xout(i-1) + h;
    end
    
    % Precompute F as recommended at exercises
    f = zeros(N+1, N+1);
    for i = 2:N
        for j = 2:N
            f(i,j) = h^2*F(xout(i), xout(j));
        end
    end
    
    while true % This should be iterations bound
        y_abs_max = 0;
        max_diff = 0;
        for i = 2:N
            for j = 2:N
                ynew(i, j) = (yold(i-1, j) + yold(i+1, j) + yold(i, j-1) + yold(i, j+1) + f(i, j))/4;
                
                y_abs_max = max(abs(ynew(i, j)), y_abs_max);
                max_diff  = max(abs(ynew(i, j) - yold(i, j)), max_diff);
            end
        end
        if max_diff / y_abs_max < epsilon
            break;
        end
        yold = ynew;
    end
    
    yout = ynew;
end