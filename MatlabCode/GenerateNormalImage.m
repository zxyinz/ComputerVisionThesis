function NormalImage = GenerateNormalImage(GrayScaleImage, Radius, CameraInfo)

[Width,Height] = size(GrayScaleImage);

VMatrix = zeros(Width,Height,3);
NormalImage = zeros(Width,Height,3);

%Projection depth image to vertex image
for seek_y = 1 : Height
    for seek_x = 1 : Width;
        Coord = [double(seek_x) double(seek_y) double(GrayScaleImage(seek_x, seek_y))];
        VMatrix(seek_x,seek_y,:) = Calc3DCoord(Coord,CameraInfo);
    end
end

R = max(Radius,1.0);

NormalWidth = Width - R;
NormalHeight = Height - R;

%Calc local normal
for seek_y = (1 + R) : NormalHeight
    for seek_x = (1 + R) : NormalWidth;
    
    %8 point
    LocalNormal = [0.0 0.0 0.0];
    
    % #1
    Vec1 =  VMatrix(seek_x,seek_y,:) -  VMatrix(seek_x - R,seek_y + R,:);
    Vec2 =  VMatrix(seek_x,seek_y,:) -  VMatrix(seek_x + 0,seek_y + 1,:);
    Normal = cross(Vec1(:)',Vec2(:)');
    LocalNormal = LocalNormal + Normal/norm(Normal);
    
    % #2
    Vec1 =  VMatrix(seek_x,seek_y,:) -  VMatrix(seek_x + 0,seek_y + R,:);
    Vec2 =  VMatrix(seek_x,seek_y,:) -  VMatrix(seek_x + R,seek_y + R,:);
    Normal = cross(Vec1(:)', Vec2(:)');
    LocalNormal = LocalNormal + Normal/norm(Normal);
    
    % #3
    Vec1 =  VMatrix(seek_x,seek_y,:) -  VMatrix(seek_x + R,seek_y + R,:);
    Vec2 =  VMatrix(seek_x,seek_y,:) -  VMatrix(seek_x + R,seek_y + 0,:);
    Normal = cross(Vec1(:)', Vec2(:)');
    LocalNormal = LocalNormal + Normal/norm(Normal);
    
    % #4
    Vec1 =  VMatrix(seek_x,seek_y,:) -  VMatrix(seek_x + R,seek_y + 0,:);
    Vec2 =  VMatrix(seek_x,seek_y,:) -  VMatrix(seek_x + R,seek_y - R,:);
    Normal = cross(Vec1(:)', Vec2(:)');
    LocalNormal = LocalNormal + Normal/norm(Normal);
    
    % #5
    Vec1 =  VMatrix(seek_x,seek_y,:) -  VMatrix(seek_x + R,seek_y - R,:);
    Vec2 =  VMatrix(seek_x,seek_y,:) -  VMatrix(seek_x + 0,seek_y - R,:);
    Normal = cross(Vec1(:)', Vec2(:)');
    LocalNormal = LocalNormal + Normal/norm(Normal);
    
    % #6
    Vec1 =  VMatrix(seek_x,seek_y,:) -  VMatrix(seek_x + 0,seek_y - R,:);
    Vec2 =  VMatrix(seek_x,seek_y,:) -  VMatrix(seek_x - R,seek_y - R,:);
    Normal = cross(Vec1(:)', Vec2(:)');
    LocalNormal = LocalNormal + Normal/norm(Normal);
    
    % #7
    Vec1 =  VMatrix(seek_x,seek_y,:) -  VMatrix(seek_x - R,seek_y - R,:);
    Vec2 =  VMatrix(seek_x,seek_y,:) -  VMatrix(seek_x - R,seek_y + 0,:);
    Normal = cross(Vec1(:)', Vec2(:)');
    LocalNormal = LocalNormal + Normal/norm(Normal);
    
    % #8
    Vec1 =  VMatrix(seek_x,seek_y,:) -  VMatrix(seek_x - R,seek_y + 0,:);
    Vec2 =  VMatrix(seek_x,seek_y,:) -  VMatrix(seek_x - R,seek_y + R,:);
    Normal = cross(Vec1(:)', Vec2(:)');
    LocalNormal = LocalNormal + Normal/norm(Normal);
    
    %Average
    LocalNormal = (LocalNormal / 8.0) * 255.0;
    NormalImage(seek_x,seek_y,:) = LocalNormal;
        
    end
end

end