function PD_main(Path)

OrigionalImage = imread(Path);
OrigionalImage = im2double(OrigionalImage);
CameraInfo = zeros(3);

%Generate image
Image = OrigionalImage;
%Image = GenerateNormalImage(rgb2gray(OrigionalImage),1, CameraInfo);

figure, imshow(Image);
HoughTransform(Image,10);

end

