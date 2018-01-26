function [] = HW_K2(task)
    F = '[y(2);2*y(2)-3*y(2)^3-2*y(1)]';
    tspan = [0, 50];
    y0 = [1; -2];
    h = 2^-9;
    [tout, yout] = RungeKutta( F,tspan,y0,h );
    switch task
        case 'b_plot'
            plot(tout, yout(:,1), 'color', 'red');
            hold on;
            plot(tout, yout(:,2), 'color', 'blue');
            hold off;
            title('Solution')
            legend('u(t)','u''(t)')
            xlabel('t');
        case 'b_table'
            format long;
            disp(tout(end-9:end));
            disp(yout(end-9:end,:));
        case 'b_curve'
            plot(yout(:,1), yout(:,2), 'color', 'red');
            title('Phase Curve')
            xlabel('u(t)');
            ylabel('u''(t)');
        case 'c'
            h = 2^-10;
            [~, yout2] = RungeKutta( F,tspan,y0,h );
            h = 2^-11;
            [~, yout4] = RungeKutta( F,tspan,y0,h );
            error=zeros(length(tout),2);
            for i = 2:length(tout)
                error(i,:)=[log2(abs((yout(i,1) - yout2(i*2 - 1,1))/(yout2(i*2 - 1,1) - yout4(i*4 - 3,1))));
                            log2(abs((yout(i,2) - yout2(i*2 - 1,2))/(yout2(i*2 - 1,2) - yout4(i*4 - 3,2))))];
                if i <= 11
                    disp([i-1, tout(i), yout(i,:), yout2(i*2-1,:), yout4(i*4-3,:),error(i,:)])
                end
            end
            plot(tout, error(:,1), 'color', 'red');
            hold on;
            plot(tout, error(:,2), 'color', 'blue');
            hold off;
            title('Rate of Convergence')
            legend('u(t)','u''(t)')
            xlabel('t');
            ylabel('alpha(t)');
    end
end

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