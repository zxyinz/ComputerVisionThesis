function HoughTransform(OrigionalImage, MaxLineNumber)

GrayImage = rgb2gray(OrigionalImage);

%Find edge
BW = edge(GrayImage,'canny');
figure, imshow(BW);

%Hough transform
[H,theta,rho] = hough(BW);

%Plot
figure, imshow(imadjust(mat2gray(H)),[],'X',theta,'Y',rho,'InitialMagnification','fit');
xlabel('\theta (degrees)'), ylabel('\rho');
axis on, axis normal, hold on;
colormap(hot)

%Find point
P = houghpeaks(H,MaxLineNumber,'threshold',ceil(0.3*max(H(:))));

%Plot
x = theta(P(:,2));
y = rho(P(:,1));
plot(x,y,'s','color','black');

%Find line
lines = houghlines(BW,theta,rho,P,'FillGap',5,'MinLength',7);

%Draw line
figure, imshow(OrigionalImage), hold on
max_len = 0;
for k = 1:length(lines)
   xy = [lines(k).point1; lines(k).point2];
   plot(xy(:,1),xy(:,2),'LineWidth',2,'Color','green');

   % Plot beginnings and ends of lines
   plot(xy(1,1),xy(1,2),'x','LineWidth',2,'Color','yellow');
   plot(xy(2,1),xy(2,2),'x','LineWidth',2,'Color','red');

   % Determine the endpoints of the longest line segment
   len = norm(lines(k).point1 - lines(k).point2);
   if ( len > max_len)
      max_len = len;
      xy_long = xy;
   end
end

% highlight the longest line segment
plot(xy_long(:,1),xy_long(:,2),'LineWidth',2,'Color','red');

end