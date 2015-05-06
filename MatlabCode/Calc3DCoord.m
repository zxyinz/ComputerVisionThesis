function Coord = Calc3DCoord(CoordXYD, ImageSize, Camera)
Coord = [CoordXYD(1)/double(ImageSize(1)) CoordXYD(2)/double(ImageSize(2)) CoordXYD(3)];
end