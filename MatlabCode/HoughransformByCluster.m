function HoughransformByCluster(OrigionalImage, ClauserNumber, MaxLineNumber)

[Width,Height,Channel] = size(OrigionalImage);

%Cluster color map
ColorMap = [255 0 0; 0 255 0; 0 0 255; 255 255 0; 255 0 255; 0 255 255;...
            192 0 0; 0 192 0; 0 0 192; 192 192 0; 192 0 192; 0 192 192;...
            128 0 0; 0 128 0; 0 0 128; 128 128 0; 128 0 128; 0 128 128;...
            64 0 0; 0 64 0; 0 0 64; 64 64 0; 64 0 64; 0 64 64;];
ClusterColorImage = zeros(Width, Height, 3);

%Apply clauser algorithm
DataSet = reshape(OrigionalImage,Width * Height, Channel);

%K-means
IndexMatrix = kmeans(DataSet,5);

%DBScan
%[IndexMatrix IndexScore] = dbscan(DataSet,10,5);

IndexMatrix = reshape(IndexMatrix,Width,Height);

%Draw color image
for seek_height = 1 : Height
    for seek_width = 1 : Width
        ColorIndex = IndexMatrix(seek_width,seek_height);
        ClusterColorImage(seek_width,seek_height,:) = ColorMap(ColorIndex,:);
    end
end
figure, imshow(ClusterColorImage);

    
%Draw origional image
figure, imshow(OrigionalImage), hold on

for seek_clauser = 1 : ClauserNumber
    
    %Generate sub image
    SubImage = zeros(Width,Height,Channel);
    SubImage(:) = double(0.0);
    
    for seek_height = 1 : Height
        for seek_width = 1 : Width
            if IndexMatrix(seek_width,seek_height) == seek_clauser
                SubImage(seek_width,seek_height,:) = OrigionalImage(seek_width,seek_height,:);
            end
        end
    end
    
    %Hough transform for sub image
    GrayImage = rgb2gray(SubImage);

    %Find edge
    BW = edge(GrayImage,'canny');

    %Hough transform
    [H,theta,rho] = hough(BW);

    %Plot
    %figure, imshow(imadjust(mat2gray(H)),[],'X',theta,'Y',rho,'InitialMagnification','fit');
    %xlabel('\theta (degrees)'), ylabel('\rho');
    %axis on, axis normal, hold on;
    %colormap(hot)

    %Find point
    P = houghpeaks(H,MaxLineNumber,'threshold',ceil(0.3*max(H(:))));

    %Plot
    %x = theta(P(:,2));
    %y = rho(P(:,1));
    %plot(x,y,'s','color','black');

    %Find line
    lines = houghlines(BW,theta,rho,P,'FillGap',5,'MinLength',7);

    %Draw line
    max_len = 0;
    for k = 1:length(lines)
       xy = [lines(k).point1; lines(k).point2];
       plot(xy(:,1),xy(:,2),'LineWidth',2,'Color','green');

       % Plot beginnings and ends of lines
       plot(xy(1,1),xy(1,2),'x','LineWidth',2,'Color','yellow');
       plot(xy(2,1),xy(2,2),'x','LineWidth',2,'Color','red');
    end
    
end %End of clauser

end