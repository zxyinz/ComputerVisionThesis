function NormalImage = GenerateNormalImage(GrayScaleImage, CameraInfo)

[Width,Height] = size(GrayScaleImage);

VMatrix = Zero(Width,Height);
NormalImage = Zero(Width,Height,3);

%Projection depth image to vertex image
for seek_y = 0 : Height
    for seek_x = 0 : Width;
        VMatrix(seek_x,seek_y) = Calc3DCoord(GrayScaleImage(seek_x, seek_y),CameraInfo);
    end
end

NormalWidth = Width - 1;
NormalHeight = Height - 1;

%Calc local normal
for seek_y = 1 : NormalHeight
    for seek_x = 1 : NormalWidth;
    
    %8 point
    LocalNormal = Zero(3);
    
    % #1
    Vec1 =  VMatrix(seek_x,seek_y) -  VMatrix(seek_x - 1,seek_y + 1);
    Vec2 =  VMatrix(seek_x,seek_y) -  VMatrix(seek_x + 0,seek_y + 1);
    Normal = dot(Vec1,Vec2);
    LocalNormal = LocalNormal + Normal/norm(Normal);
    
    % #2
    Vec1 =  VMatrix(seek_x,seek_y) -  VMatrix(seek_x + 0,seek_y + 1);
    Vec2 =  VMatrix(seek_x,seek_y) -  VMatrix(seek_x + 1,seek_y + 1);
    Normal = dot(Vec1,Vec2);
    LocalNormal = LocalNormal + Normal/norm(Normal);
    
    % #3
    Vec1 =  VMatrix(seek_x,seek_y) -  VMatrix(seek_x + 1,seek_y + 1);
    Vec2 =  VMatrix(seek_x,seek_y) -  VMatrix(seek_x + 1,seek_y + 0);
    Normal = dot(Vec1,Vec2);
    LocalNormal = LocalNormal + Normal/norm(Normal);
    
    % #4
    Vec1 =  VMatrix(seek_x,seek_y) -  VMatrix(seek_x + 1,seek_y + 0);
    Vec2 =  VMatrix(seek_x,seek_y) -  VMatrix(seek_x + 1,seek_y - 1);
    Normal = dot(Vec1,Vec2);
    LocalNormal = LocalNormal + Normal/norm(Normal);
    
    % #5
    Vec1 =  VMatrix(seek_x,seek_y) -  VMatrix(seek_x + 1,seek_y - 1);
    Vec2 =  VMatrix(seek_x,seek_y) -  VMatrix(seek_x + 0,seek_y - 1);
    Normal = dot(Vec1,Vec2);
    LocalNormal = LocalNormal + Normal/norm(Normal);
    
    % #6
    Vec1 =  VMatrix(seek_x,seek_y) -  VMatrix(seek_x + 0,seek_y - 1);
    Vec2 =  VMatrix(seek_x,seek_y) -  VMatrix(seek_x - 1,seek_y - 1);
    Normal = dot(Vec1,Vec2);
    LocalNormal = LocalNormal + Normal/norm(Normal);
    
    % #7
    Vec1 =  VMatrix(seek_x,seek_y) -  VMatrix(seek_x - 1,seek_y - 1);
    Vec2 =  VMatrix(seek_x,seek_y) -  VMatrix(seek_x - 1,seek_y + 0);
    Normal = dot(Vec1,Vec2);
    LocalNormal = LocalNormal + Normal/norm(Normal);
    
    % #8
    Vec1 =  VMatrix(seek_x,seek_y) -  VMatrix(seek_x - 1,seek_y + 0);
    Vec2 =  VMatrix(seek_x,seek_y) -  VMatrix(seek_x - 1,seek_y + 1);
    Normal = dot(Vec1,Vec2);
    LocalNormal = LocalNormal + Normal/norm(Normal);
    
    %Average
    NormalImage(seek_x,seek_y) = (LocalNormal / 8.0) * 255.0;
        
    end
end

end