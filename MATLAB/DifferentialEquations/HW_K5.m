function [] = HW_K5()
    F = @(x, t) x/(1+t)^2;
    u0 = @(x) 0;
    mu1 = @(t) 0;
    mu2 = @(t) 2*t/(1+t);
    l = 1;
    T = 2;
    h = 0.1;
    tau = 0.005;
    
    solution = @(x, t) t*x/(1+t);
    
    table = zeros(11, 6);
    time = 2.0;
    layer = time / tau + 1;
    
    [xout, tout, yout] = ExplicitMethod(F, u0, mu1, mu2, l, T, h, tau);
    table(:,1) = xout;
    table(:,2) = yout(:, layer);
    
    [~, ~, yout] = ImplicitMethod(F, u0, mu1, mu2, l, T, h, tau);
    table(:,3) = yout(:, layer);
    
    yout = ExactSolution(xout, tout, solution);
    table(:,4) = yout(:, layer);
    
    table(:,5) = table(:,3) - table(:,2);
    plot(xout, table(:, 5), 'LineStyle', '--');
    hold on;
    table(:,6) = table(:,4) - table(:,2);
    plot(xout, table(:, 6))
    title(sprintf('Time = %.1f', time))
    xlabel('xi');
    ylabel('Ei');
    hold off;
    
    disp(tout(layer));
    fig = figure;
    uitable(fig, 'Data', table);
end

function yout = ExactSolution(xout, tout, solution)
    yout = zeros(length(xout), length(tout));
    for j = 1:length(tout)
        for i = 1:length(xout)
            yout(i, j) = solution(xout(i), tout(j));
        end
    end
end

function [xout, tout, yout] = ExplicitMethod(F, u0, mu1, mu2, l, T, h, tau)
    N = round(l / h) + 1;
    M = round(T / tau) + 1;
    
    xout = zeros(N, 1);
    tout = zeros(M, 1);
    yout = zeros(N, M);
    
    % Boundary aproximation (2')
    for i = 1:N
        xout(i) = h*(i - 1);
        yout(i, 1) = u0(xout(i));
    end
    
    % Boundary aproximation (3')
    for j = 1:M
        tout(j) = tau*(j - 1);
        yout(1, j) = mu1(tout(j));
    end
    
    % Aproximation (1')
    gamma = tau / h^2;
    for j = 1:M-1
        for i = 2:N-1
            yout(i, j+1) = gamma*(yout(i+1, j)-2*yout(i, j)+yout(i-1, j)) + yout(i, j) + tau*F(xout(i), tout(j));
        end
        % Boundary aproximation (4')
        yout(N, j+1) = (2*tau/h)*mu2(tout(j)) + (1-2*tau/h-2*tau/h^2)*yout(N, j) + tau*F(xout(N), tout(j)) + 2*gamma*yout(N-1, j);
    end
end

function [xout, tout, yout] = ImplicitMethod(F, u0, mu1, mu2, l, T, h, tau)
    N = round(l / h) + 1;
    M = round(T / tau) + 1;
    
    xout = zeros(N, 1);
    tout = zeros(M, 1);
    yout = zeros(N, M);
    
    % Boundary aproximation (2')
    tout(1) = 0;
    for i = 1:N
        xout(i) = h*(i - 1);
        yout(i, 1) = u0(xout(i));
    end
    
    % Aproximation (1')
    for j = 1:M-1
        tout(j+1) = tau*j;
        yout(:, j+1) = ThomasAlgorithm(xout, yout(:, j), tout(j+1), F, mu1, mu2, h, tau);
    end
end

function yout = ThomasAlgorithm(xout, old, t, F, mu1, mu2, h, tau)
    N = length(xout);

    % allocating space
    alpha = zeros(N, 1);
    beta  = zeros(N, 1);
    yout  = zeros(N, 1);
        
    % Boundary aproximation (3')
    F0 = mu1(t);
    c0 = 1;
    b0 = 0;
    alpha(1) =  b0 / c0;
    beta(1)  = -F0 / c0;
    
    gamma = tau/h^2;
    for i = 1:N-2
        Fi = -tau*F(xout(i+1), t)-old(i+1);
        ci = 1+2*gamma;
        bi = gamma;
        ai = gamma;
        alpha(i+1) =                 bi / (ci - ai*alpha(i));
        beta(i+1)  = (-Fi + ai*beta(i)) / (ci - ai*alpha(i));
    end
    
    % Boundary aproximation (4')
    FN = -(h*old(N))/(2*tau)-mu2(t)-h/2*F(xout(N), t);
    cN = 1+1/h+h/(2*tau);
    aN = 1/h;
    yout(N) = (-FN + aN*beta(N-1)) / (cN - aN*alpha(N-1));        
    
    for i = N-1:-1:1
        yout(i) = alpha(i)*yout(i+1) + beta(i);
    end
end