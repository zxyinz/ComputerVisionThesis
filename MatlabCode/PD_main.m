function PD_main(Path)

clf

OrigionalImage = imread(Path);
OrigionalImage = im2double(OrigionalImage);
CameraInfo = zeros(3);

%Generate image
Image = OrigionalImage;
%Image = GenerateNormalImage(rgb2gray(OrigionalImage),1, CameraInfo);

%Blur Image
%BlurMatrix = fspecial('disk',3);
%Image = imfilter(Image,BlurMatrix,'replicate');

figure, imshow(Image);
%HoughTransform(Image,50);
HoughransformByCluster(Image,8,4);

end

