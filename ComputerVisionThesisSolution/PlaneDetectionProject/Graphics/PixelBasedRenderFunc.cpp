#include"PixelBasedRenderFunc.h"
using namespace San;
using namespace San::Graphics;
void San::Graphics::ClearScreenRGBA(sfloat* p_buffer, const uint32 width, const uint32 height, const SANCOLORF &Color)
{
	if (p_buffer == nullptr){ return; }

	int32 size = width*height * 4;

#pragma omp parallel for
	for (int32 seek = 0; seek < size; seek = seek + 4)
	{
		p_buffer[seek] = Color.b;
		p_buffer[seek + 1] = Color.g;
		p_buffer[seek + 2] = Color.r;
		p_buffer[seek + 3] = Color.a;
	}
}
void San::Graphics::DrawImageRGBA(sfloat* p_buffer, const uint32 buf_width, const uint32 buf_height, sfloat* p_img, const uint32 img_width, const uint32 img_height, const SVECTOR3 &pos, bool bUseAlpha)
{
	if ((p_buffer == nullptr) || (p_img == nullptr)){ return; }

	if (((pos.x + img_width) < 0.0) || ((pos.y + img_height) < 0.0)){ return; }

	const int32 pos_x = pos.x < 0.0 ? 0 : pos.x;
	const int32 pos_y = pos.y < 0.0 ? 0 : pos.y;

	const int32 img_pos_x = pos.x < 0.0 ? -pos.x : 0;
	const int32 img_pos_y = pos.y < 0.0 ? -pos.y : 0;

	const int32 bound_x = (pos.x + img_width) > buf_width ? buf_width : (pos.x + img_width);
	const int32 bound_y = (pos.y + img_height) > buf_height ? buf_height : (pos.y + img_height);

	if (bUseAlpha)
	{
#pragma opm parallel for
		for (int32 seek_y = pos_y; seek_y < bound_y; seek_y = seek_y + 1)
		{
			const int32 buf_row_offset = seek_y * (buf_width << 2) + (pos_x << 2);
			const int32 img_row_offset = (img_height - (img_pos_y + (seek_y - pos_y)) - 1) * (img_width << 2) + (img_pos_x << 2);

			sfloat* p_dest = p_buffer + buf_row_offset;
			sfloat* p_src = p_img + img_row_offset;

			for (int32 seek_x = pos_x; seek_x < bound_x; seek_x = seek_x + 1)
			{
				const sfloat alpha = p_src[3] / 255.0;
				const sfloat ont_min_alpha = 1.0 - alpha;

				p_dest[0] = p_dest[0] * ont_min_alpha + p_src[2] * alpha;
				p_dest[1] = p_dest[1] * ont_min_alpha + p_src[1] * alpha;
				p_dest[2] = p_dest[2] * ont_min_alpha + p_src[0] * alpha;
				p_dest[3] = p_dest[3] * ont_min_alpha + p_src[3] * alpha;

				p_dest = p_dest + 4;
				p_src = p_src + 4;
			}
		}
	}
	else
	{
#pragma opm parallel for
		for (int32 seek_y = pos_y; seek_y < bound_y; seek_y = seek_y + 1)
		{
			const int32 buf_row_offset = seek_y * (buf_width << 2) + (pos_x << 2);
			const int32 img_row_offset = (img_height - (img_pos_y + (seek_y - pos_y)) - 1) * (img_width << 2) + (img_pos_x << 2);

			sfloat* p_dest = p_buffer + buf_row_offset;
			sfloat* p_src = p_img + img_row_offset;

			for (int32 seek_x = pos_x; seek_x < bound_x; seek_x = seek_x + 1)
			{
				p_dest[0] = p_src[2];
				p_dest[1] = p_src[1];
				p_dest[2] = p_src[0];
				p_dest[3] = p_src[3];

				p_dest = p_dest + 4;
				p_src = p_src + 4;
			}
		}
	}
}
void San::Graphics::DrawImageRGB(sfloat* p_buffer, const uint32 buf_width, const uint32 buf_height, sfloat* p_img, const uint32 img_width, const uint32 img_height, const SVECTOR3 &pos)
{
	if ((p_buffer == nullptr) || (p_img == nullptr)){ return; }

	if (((pos.x + img_width) < 0.0) || ((pos.y + img_height) < 0.0)){ return; }

	const int32 pos_x = pos.x < 0.0 ? 0 : pos.x;
	const int32 pos_y = pos.y < 0.0 ? 0 : pos.y;

	const int32 img_pos_x = pos.x < 0.0 ? -pos.x : 0;
	const int32 img_pos_y = pos.y < 0.0 ? -pos.y : 0;

	const int32 bound_x = (pos.x + img_width) > buf_width ? buf_width : (pos.x + img_width);
	const int32 bound_y = (pos.y + img_height) > buf_height ? buf_height : (pos.y + img_height);

#pragma opm parallel for
	for (int32 seek_y = pos_y; seek_y < bound_y; seek_y = seek_y + 1)
	{
		const int32 buf_row_offset = seek_y * (buf_width << 2) + (pos_x << 2);
		const int32 img_row_offset = (img_height - (img_pos_y + (seek_y - pos_y)) - 1) * (img_width * 3) + (img_pos_x * 3);

		sfloat* p_dest = p_buffer + buf_row_offset;
		sfloat* p_src = p_img + img_row_offset;

		for (int32 seek_x = pos_x; seek_x < bound_x; seek_x = seek_x + 1)
		{
			p_dest[0] = p_src[2];
			p_dest[1] = p_src[1];
			p_dest[2] = p_src[0];

			p_dest = p_dest + 4;
			p_src = p_src + 3;
		}
	}
}
void San::Graphics::DrawImageGray(sfloat* p_buffer, const uint32 buf_width, const uint32 buf_height, sfloat* p_img, const uint32 img_width, const uint32 img_height, const SVECTOR3 &pos)
{
	if ((p_buffer == nullptr) || (p_img == nullptr)){ return; }

	if (((pos.x + img_width) < 0.0) || ((pos.y + img_height) < 0.0)){ return; }

	const int32 pos_x = pos.x < 0.0 ? 0 : pos.x;
	const int32 pos_y = pos.y < 0.0 ? 0 : pos.y;

	const int32 img_pos_x = pos.x < 0.0 ? -pos.x : 0;
	const int32 img_pos_y = pos.y < 0.0 ? -pos.y : 0;

	const int32 bound_x = (pos.x + img_width) > buf_width ? buf_width : (pos.x + img_width);
	const int32 bound_y = (pos.y + img_height) > buf_height ? buf_height : (pos.y + img_height);

#pragma opm parallel for
	for (int32 seek_y = pos_y; seek_y < bound_y; seek_y = seek_y + 1)
	{
		const int32 buf_row_offset = seek_y * (buf_width << 2) + (pos_x << 2);
		const int32 img_row_offset = (img_height - (img_pos_y + (seek_y - pos_y)) - 1) * img_width + img_pos_x;

		sfloat* p_dest = p_buffer + buf_row_offset;
		sfloat* p_src = p_img + img_row_offset;

		for (int32 seek_x = pos_x; seek_x < bound_x; seek_x = seek_x + 1)
		{
			sfloat val = p_src[0];

			p_dest[0] = val;
			p_dest[1] = val;
			p_dest[2] = val;

			p_dest = p_dest + 4;
			p_src = p_src + 1;
		}
	}
}
void San::Graphics::DrawCrossRGBA(sfloat* p_buffer, const uint32 width, uint32 height, const SVECTOR3 pos, const SANCOLORF color, const uint32 size,const uint32 thickness)
{
	if (p_buffer == nullptr){ return; }
	if ((width == 0) || (height == 0)){ return; }

	int32 pos_x = 0;
	int32 pos_y = 0;
	int32 pos_bound = 0;

	for (uint32 seek_t = 0; seek_t < thickness; seek_t = seek_t + 1)
	{
		pos_y = pos.y + seek_t - thickness / 2;
		if ((pos_y < 0) || (pos_y >= height))
		{
			continue;
		}
		pos_y = pos_y*width * 4;
		pos_x = pos.x - size/2;
		pos_bound = pos_x + size;
		pos_bound = pos_bound > width ? width : pos_bound;
		for (int32 seek_x = pos_x; seek_x < pos_bound; seek_x = seek_x + 1)
		{
			if (seek_x > 0)
			{
				p_buffer[pos_y + seek_x * 4] = color.b;
				p_buffer[pos_y + seek_x * 4 + 1] = color.g;
				p_buffer[pos_y + seek_x * 4 + 2] = color.r;
				p_buffer[pos_y + seek_x * 4 + 3] = color.a;
			}
		}
	}
	for (uint32 seek_t = 0; seek_t < thickness; seek_t = seek_t + 1)
	{
		pos_x = pos.x + seek_t - thickness / 2;
		if ((pos_x < 0) || (pos_x >= width))
		{
			continue;
		}
		pos_x = pos_x * 4;
		pos_y = pos.y - size/2;
		pos_bound = pos_y + size;
		pos_bound = pos_bound > height ? height : pos_bound;
		for (int32 seek_y = pos_y; seek_y < pos_bound; seek_y = seek_y + 1)
		{
			if (seek_y > 0)
			{
				p_buffer[seek_y*width * 4 + pos_x] = color.b;
				p_buffer[seek_y*width * 4 + pos_x + 1] = color.g;
				p_buffer[seek_y*width * 4 + pos_x + 2] = color.r;
				p_buffer[seek_y*width * 4 + pos_x + 3] = color.a;
			}
		}
	}
}
void San::Graphics::DrawDotRGBA(sfloat* p_buffer, const uint32 width, uint32 height, const SVECTOR3 pos, const SANCOLORF color, const sfloat radius)
{
	if (p_buffer == nullptr){ return; }
	if ((width == 0) || (height == 0) || (radius <= 0.0f)){ return; }

	int32 pos_x = pos.x - radius;
	int32 pos_y = pos.y - radius;
	int32 bound_x = pos.x + radius;
	int32 bound_y = pos.y + radius;

	if ((bound_x < 0) || (bound_y < 0)){ return; }

	pos_x = pos_x<0 ? 0 : pos_x;
	pos_y = pos_y<0 ? 0 : pos_y;
	bound_x = bound_x > width ? width : bound_x;
	bound_y = bound_y > height ? height : bound_y;

	sfloat rate = 0.0f;
	SANCOLORF Col = color;

	uint32 pre_row_width = width << 2;
	uint32 row_offset = 0;
	uint32 col_offset = 0;

	for (int32 seeky = pos_y; seeky < bound_y; seeky = seeky + 1)
	{
		for (int32 seekx = pos_x; seekx < bound_x; seekx = seekx + 1)
		{
			rate = (seekx - pos.x)*(seekx - pos.x) + (seeky - pos.y)*(seeky - pos.y);
			rate = sqrt(rate) / radius;

			if (rate < 1.0f)
			{
				p_buffer[row_offset + col_offset] = Col.b;
				p_buffer[row_offset + col_offset + 1] = Col.g;
				p_buffer[row_offset + col_offset + 2] = Col.r;
				p_buffer[row_offset + col_offset + 3] = Col.a;
			}

			col_offset = col_offset + 4;
		}

		row_offset = row_offset + pre_row_width;
	}
}
void San::Graphics::DrawRectangleRGBA(sfloat* p_buffer, const uint32 buf_width, const uint32 buf_height, const uint32 quadr_width, const uint32 quadr_height, const SANCOLORF color, const SVECTOR3 &pos)
{
	if (p_buffer == nullptr){ return; }
	if ((buf_width == 0) || (buf_height == 0)){ return; }

	int32 pos_x = pos.x;
	int32 pos_y = pos.y;
	int32 bound_x = pos.x + quadr_width;
	int32 bound_y = pos.y + quadr_height;

	if ((bound_x < 0) || (bound_y < 0)){ return; }

	pos_x = pos_x<0 ? 0 : pos_x;
	pos_y = pos_y<0 ? 0 : pos_y;
	bound_x = bound_x > buf_width ? buf_width : bound_x;
	bound_y = bound_y > buf_height ? buf_height : bound_y;

	SANCOLORF Col = color;

	uint32 pre_row_width = buf_width << 2;
	uint32 row_offset = 0;
	uint32 col_offset = 0;

	for (int32 seeky = pos_y; seeky < bound_y; seeky = seeky + 1)
	{
		col_offset = pos_x << 2;
		for (int32 seekx = pos_x; seekx < bound_x; seekx = seekx + 1)
		{
			p_buffer[row_offset + col_offset] = Col.b;
			p_buffer[row_offset + col_offset + 1] = Col.g;
			p_buffer[row_offset + col_offset + 2] = Col.r;
			p_buffer[row_offset + col_offset + 3] = Col.a;

			col_offset = col_offset + 4;
		}

		row_offset = row_offset + pre_row_width;
	}
}
void San::Graphics::DrawQuadrRGBA(sfloat* p_buffer, const uint32 buf_width, const uint32 buf_height, const uint32 quadr_width, const uint32 quadr_height, const SANCOLORF color, const SVECTOR3 &pos, const uint32 size)
{
	if (p_buffer == nullptr)
	{
		return;
	}
	uint32 pos_x = 0;
	uint32 pos_y = 0;
	uint32 bound_x = pos.x + quadr_width;
	uint32 bound_y = pos.y + size;
	bound_x = bound_x > buf_width ? buf_width : bound_x;
	bound_y = bound_y > buf_height ? buf_height : bound_y;
	for (int32 seek_y = pos.y; seek_y < bound_y; seek_y = seek_y + 1)
	{
		if (seek_y < 0)
		{
			continue;
		}
		pos_y = seek_y*buf_width * 4;
		for (int32 seek_x = pos.x; seek_x < bound_x; seek_x = seek_x + 1)
		{
			if (seek_x < 0)
			{
				continue;
			}
			if (seek_x >= buf_width)
			{
				break;
			}
			pos_x = seek_x * 4;
			p_buffer[pos_y + pos_x] = color.b;
			p_buffer[pos_y + pos_x+1] = color.g;
			p_buffer[pos_y + pos_x+2] = color.r;
			p_buffer[pos_y + pos_x+3] = color.a;
		}
	}
	bound_y = pos.y + quadr_height + size;
	bound_y = bound_y > buf_height ? buf_height : bound_y;
	for (int32 seek_y = pos.y + quadr_height; seek_y < bound_y; seek_y = seek_y + 1)
	{
		if (seek_y < 0)
		{
			continue;
		}
		if (seek_y >= buf_height)
		{
			break;
		}
		pos_y = seek_y*buf_width * 4;
		for (int32 seek_x = pos.x; seek_x < bound_x; seek_x = seek_x + 1)
		{
			if (seek_x < 0)
			{
				continue;
			}
			if (seek_x >= buf_width)
			{
				break;
			}
			pos_x = seek_x * 4;
			p_buffer[pos_y + pos_x] = color.b;
			p_buffer[pos_y + pos_x + 1] = color.g;
			p_buffer[pos_y + pos_x + 2] = color.r;
			p_buffer[pos_y + pos_x + 3] = color.a;
		}
	}
	bound_x = pos.x + size;
	bound_y = pos.y + quadr_height;
	bound_x = bound_x > buf_width ? buf_width : bound_x;
	bound_y = bound_y > buf_height ? buf_height : bound_y;
	for (int32 seek_x = pos.x; seek_x < bound_x; seek_x = seek_x + 1)
	{
		if (seek_x < 0)
		{
			continue;
		}
		if (seek_x >= buf_width)
		{
			break;
		}
		pos_x = seek_x* 4;
		for (int32 seek_y = pos.y; seek_y < quadr_height; seek_y = seek_y + 1)
		{
			if (seek_y < 0)
			{
				continue;
			}
			if (seek_y >= buf_height)
			{
				break;
			}
			pos_y = seek_y *buf_width * 4;
			p_buffer[pos_y + pos_x] = color.b;
			p_buffer[pos_y + pos_x + 1] = color.g;
			p_buffer[pos_y + pos_x + 2] = color.r;
			p_buffer[pos_y + pos_x + 3] = color.a;
		}
	}
	bound_x = pos.x + quadr_width + size;
	bound_x = bound_x > buf_width ? buf_width : bound_x;
	for (int32 seek_x = pos.x + quadr_width; seek_x < bound_x; seek_x = seek_x + 1)
	{
		if (seek_x < 0)
		{
			continue;
		}
		if (seek_x >= buf_width)
		{
			break;
		}
		pos_x = seek_x * 4;
		for (int32 seek_y = pos.y; seek_y < quadr_height; seek_y = seek_y + 1)
		{
			if (seek_y < 0)
			{
				continue;
			}
			if (seek_y >= buf_height)
			{
				break;
			}
			pos_y = seek_y *buf_width * 4;
			p_buffer[pos_y + pos_x] = color.b;
			p_buffer[pos_y + pos_x + 1] = color.g;
			p_buffer[pos_y + pos_x + 2] = color.r;
			p_buffer[pos_y + pos_x + 3] = color.a;
		}
	}
}
void San::Graphics::DrawLineRGBA(sfloat* p_buffer, const uint32 buf_width, const uint32 buf_height, const SVECTOR3 pos_begin, const SVECTOR3 pos_end, const SANCOLORF color, const uint32 size)
{
	if (p_buffer == nullptr)
	{
		return;
	}
	SVECTOR3 DirX = pos_end - pos_begin;
	SVECTOR3 DirY = pos_end - pos_begin;
	SVECTOR3 length = SVECTOR3(pos_end.x - pos_begin.x, pos_end.y - pos_begin.y, 0.0f);
	length.x = length.x > 0.0f ? length.x : length.x*(-1.0f);
	length.y = length.y > 0.0f ? length.y : length.y*(-1.0f);
	DirX = SVECTOR3(DirX.x / length.x, DirX.y / length.x, 0.0f);
	DirY = SVECTOR3(DirY.x / length.y, DirY.y / length.y, 0.0f);
//#pragma omp parallel for
	uint32 offset = size / 2;
	for (uint32 seek_w = 0; seek_w < size; seek_w = seek_w + 1)
	{
//#pragma omp parallel for
		for (int32 seek = 0; seek < (int32)length.x; seek = seek + 1)
		{
			uint32 pos_x = (pos_begin.x - offset + seek_w) + seek*DirX.x;
			uint32 pos_y = pos_begin.y + (DirX.y*seek);
			if ((pos_x < 0) || (pos_x >= buf_width) || (pos_y < 0) || (pos_y >= buf_height))
			{
				break;
			}
			pos_y = pos_y*buf_width * 4;
			pos_x = pos_x * 4;
			p_buffer[pos_y + pos_x] = color.b;
			p_buffer[pos_y + pos_x + 1] = color.g;
			p_buffer[pos_y + pos_x + 2] = color.r;
			p_buffer[pos_y + pos_x + 3] = color.a;
		}
//#pragma omp parallel for
		for (int32 seek = 0; seek < (int32)length.y; seek = seek + 1)
		{
			uint32 pos_x = (pos_begin.x - offset + seek_w) + seek*DirY.x;
			uint32 pos_y = pos_begin.y + (seek*DirY.y);
			if ((pos_x < 0) || (pos_x >= buf_width) || (pos_y < 0) || (pos_y >= buf_height))
			{
				break;
			}
			pos_y = pos_y*buf_width * 4;
			pos_x = pos_x * 4;
			p_buffer[pos_y + pos_x] = color.b;
			p_buffer[pos_y + pos_x + 1] = color.g;
			p_buffer[pos_y + pos_x + 2] = color.r;
			p_buffer[pos_y + pos_x + 3] = color.a;
		}//*/
	}
}
void San::Graphics::UpdateCamera(PIXCAMERA &Camera, uint32 ScreenWidth, uint32 ScreenHeight)
{
	SVECTOR3 CamNor, NearOrigion, ScreenSize;
	sfloat PerPixLength;
	ScreenSize = SVECTOR3(ScreenWidth, ScreenHeight, 0.0f);
	Camera.Dir = Camera.LookAt-Camera.Coord;
	Camera.Dir = gloNormalize(Camera.Dir);
	Camera.UpDir = gloNormalize(Camera.UpDir);
	CamNor = gloNormalize(gloCross(Camera.Dir, Camera.UpDir));
	Camera.UpDir = gloNormalize(gloCross(CamNor, Camera.Dir));
	PerPixLength = Camera.Near*tan(S_PI*((Camera.Cutoff *0.5f) / 180.0f))*2.0f / ScreenWidth;
	NearOrigion = Camera.Dir + ((Camera.UpDir*(ScreenHeight / ScreenWidth)) - CamNor)*tan(S_PI*((Camera.Cutoff * 0.5f) / 180.0f));
	NearOrigion = Camera.Coord + (NearOrigion*Camera.Near);
	Camera.ScreenVec[0] = NearOrigion;
	Camera.ScreenVec[1] = NearOrigion + CamNor*ScreenWidth*PerPixLength;
	Camera.ScreenVec[2] = NearOrigion - Camera.UpDir*ScreenHeight*PerPixLength;
	Camera.ScreenVec[3] = Camera.ScreenVec[1] - Camera.UpDir*ScreenHeight*PerPixLength;
}
sfloat San::Graphics::GetHitPoint(const SVECTOR3* pRayCoord, const SVECTOR3* pRayDir, const SVECTOR3* pVec1, const SVECTOR3* pVec2, const SVECTOR3* pVec3, const SVECTOR3* pNormal, SVECTOR3* pHitPoint)
{
	SVECTOR3 VecRes, E_BA, E_CA, E_RA;
	sfloat RayDisPow = 0.0f;
	sfloat UPow, VPow;
	sfloat Dot_BABA, Dot_CACA, Dot_BACA, Dot_CABA, Dot_RABA, Dot_RACA;
	sfloat Base = 0.0f;
	sfloat Dis = 0.0f;
	VecRes.x = 0.0f;
	VecRes.y = 0.0f;
	VecRes.z = 0.0f;
	RayDisPow = pNormal->x*(pRayCoord->x - pVec1->x) + pNormal->y*(pRayCoord->y - pVec1->y) + pNormal->z*(pRayCoord->z - pVec1->z);
	Dis = pNormal->x*pRayDir->x + pNormal->y*pRayDir->y + pNormal->z*pRayDir->z;
	if (Dis == 0.0f)
	{
		return -1.0f;
	}
	RayDisPow = -RayDisPow / Dis;
	if (RayDisPow < 0.0001f)
	{
		return -1.0f;
	}
	VecRes = *pRayCoord + (*pRayDir*RayDisPow);
	E_BA = *pVec2 - *pVec1;
	E_CA = *pVec3 - *pVec1;
	E_RA = VecRes - *pVec1;
	Dot_BABA = gloDot(E_BA, E_BA);
	Dot_CACA = gloDot(E_CA, E_CA);
	Dot_BACA = gloDot(E_BA, E_CA);
	Dot_CABA = gloDot(E_CA, E_BA);
	Dot_RABA = gloDot(E_RA, E_BA);
	Dot_RACA = gloDot(E_RA, E_CA);
	Base = Dot_BABA*Dot_CACA - Dot_BACA*Dot_CABA;
	if (Base == 0.0f)
	{
		return -1.0f;
	}
	UPow = (Dot_CACA*Dot_RABA - Dot_CABA*Dot_RACA) / Base;
	VPow = (Dot_BABA*Dot_RACA - Dot_BACA*Dot_RABA) / Base;
	if ((UPow<0.0f) || (UPow>1.0f))
	{
		return -1.0f;
	}
	if ((VPow<0.0f) || (VPow>1.0f))
	{
		return -1.0f;
	}
	if ((UPow + VPow) > 1.0f)
	{
		return -1.0f;
	}
	if (pHitPoint != nullptr)
	{
		*pHitPoint = VecRes;
	}
	return RayDisPow;
}
SVECTOR3 San::Graphics::Interpolation(const SVECTOR3* pVecSrc, const SVECTOR3* pVec1, const SVECTOR3* pVec2, const SVECTOR3* pVec3)
{
	SVECTOR3 V21, V31, VS1;
	sfloat DotUU, DotVV, DotUV, DotWU, DotWV;
	sfloat rate_v, rate_n;
	SVECTOR3 Res;
	Res.x = 0;
	Res.y = 0;
	Res.z = 0;
	V21 = *pVec2 - *pVec1;
	V31 = *pVec3 - *pVec1;
	VS1 = *pVecSrc - *pVec1;
	DotUU = gloDot(V21, V21);
	DotVV = gloDot(V31, V31);
	DotUV = gloDot(V21, V31);
	DotWU = gloDot(VS1, V21);
	DotWV = gloDot(VS1, V31);
	rate_v = (DotUV*DotWV - DotVV*DotWU) / (DotUV*DotUV - DotUU*DotVV);
	rate_n = (DotUV*DotWU - DotUU*DotWV) / (DotUV*DotUV - DotUU*DotVV);
	Res.x = rate_v;
	Res.y = rate_n;
	Res.z = 0.0f;
	return Res;
}
SVECTOR3 San::Graphics::GetScreenCoord(const SVECTOR3 Coord, const PIXCAMERA Camera, uint32 ScreenWidth, uint32 ScreenHeight)
{
	SVECTOR3 RayDir = Camera.Coord - Coord;
	RayDir = gloNormalize(RayDir);
	SVECTOR3 HitPoint;
	sfloat Distance;
	SVECTOR3 coord2D;
	Distance = San::Graphics::GetHitPoint(&Coord, &RayDir, &(Camera.ScreenVec[0]), &(Camera.ScreenVec[1]), &(Camera.ScreenVec[2]), &(Camera.Dir), &HitPoint);
	if (Distance >= 0.0f)
	{
		coord2D = San::Graphics::Interpolation(&HitPoint, &(Camera.ScreenVec[0]), &(Camera.ScreenVec[1]), &(Camera.ScreenVec[2]));
		coord2D.x = coord2D.x*ScreenWidth;
		coord2D.y = coord2D.y*ScreenHeight;
		coord2D.z = Distance;
		return coord2D;
	}
	Distance = San::Graphics::GetHitPoint(&Coord, &RayDir, &(Camera.ScreenVec[3]), &(Camera.ScreenVec[2]), &(Camera.ScreenVec[1]), &(Camera.Dir), &HitPoint);
	if (Distance >= 0.0f)
	{
		coord2D = San::Graphics::Interpolation(&HitPoint, &(Camera.ScreenVec[3]), &(Camera.ScreenVec[2]), &(Camera.ScreenVec[1]));
		coord2D.x = coord2D.x*ScreenWidth;
		coord2D.y = coord2D.y*ScreenHeight;
		coord2D.x = ScreenWidth - coord2D.x;
		coord2D.y = ScreenHeight - coord2D.y;
		coord2D.z = Distance;
		return coord2D;
	}
	return SVECTOR3(-1.0f, -1.0f, -1.0f);
}
void San::Graphics::DrawDot3DRGBA(sfloat* p_buffer, const uint32 width, uint32 height, const SVECTOR3 pos, const SANCOLORF color, sfloat radius, const PIXCAMERA camera, const SVECTOR3 offset)
{
	if (p_buffer == nullptr)
	{
		return;
	}
	if ((width == 0) || (height == 0) || (radius <= 0.0f))
	{
		return;
	}
	SVECTOR3 coord = GetScreenCoord(pos, camera, width, height);
	if (coord.z < 0.0f)
	{
		return;
	}
	coord.x = coord.x + offset.x;
	coord.y = coord.y + offset.y;
	int32 pos_x = coord.x - radius;
	int32 pos_y = coord.y - radius;
	int32 bound_x = coord.x + radius;
	int32 bound_y = coord.y + radius;
	if ((bound_x < 0) || (bound_y < 0))
	{
		return;
	}
	pos_x = pos_x < 0 ? 0 : pos_x;
	pos_y = pos_y<0 ? 0 : pos_y;
	bound_x = bound_x > width ? width : bound_x;
	bound_y = bound_y > height ? height : bound_y;
	sfloat rate = 0.0f;
	SANCOLORF Col = color;
	for (int32 seeky = pos_y; seeky < bound_y; seeky = seeky + 1)
	{
		for (int32 seekx = pos_x; seekx < bound_x; seekx = seekx + 1)
		{
			rate = (seekx - coord.x)*(seekx - coord.x) + (seeky - coord.y)*(seeky - coord.y);
			rate = sqrt(rate) / radius;
			if (rate < 1.0f)
			{
				if (p_buffer[seeky*width * 4 + seekx * 4 + 3] >= coord.z)
				{
					p_buffer[seeky*width * 4 + seekx * 4] = Col.b;
					p_buffer[seeky*width * 4 + seekx * 4 + 1] = Col.g;
					p_buffer[seeky*width * 4 + seekx * 4 + 2] = Col.r;
					p_buffer[seeky*width * 4 + seekx * 4 + 3] = coord.z;
				}
			}
		}
	}
}
void San::Graphics::DrawLine3DRGBA(sfloat* p_buffer, const uint32 buf_width, const uint32 buf_height, const SVECTOR3 coord_begin, const SVECTOR3 coord_end, const SANCOLORF color, const uint32 size, const PIXCAMERA camera, const SVECTOR3 offset)
{
	if (p_buffer == nullptr)
	{
		return;
	}
	SVECTOR3 vec_begin = GetScreenCoord(coord_begin, camera, buf_width, buf_height);
	SVECTOR3 vec_end = GetScreenCoord(coord_end, camera, buf_width, buf_height);
	vec_begin = vec_begin + offset;
	vec_end = vec_end + offset;
	if ((vec_begin.z < 0.0f) || (vec_end.z < 0.0f))
	{
		return;
	}
	//SVECTOR3 DirX = SVECTOR3(vec_begin.x, vec_begin.y, 0.0f) - SVECTOR3(vec_end.x, vec_end.y, 0.0f);
	//SVECTOR3 DirY = SVECTOR3(vec_begin.x, vec_begin.y, 0.0f) - SVECTOR3(vec_end.x, vec_end.y, 0.0f);
	SVECTOR3 DirX = vec_end - vec_begin;
	SVECTOR3 DirY = vec_end - vec_begin;
	SVECTOR3 length = SVECTOR3(vec_end.x - vec_begin.x, vec_end.y - vec_begin.y, 0.0f);
	length.x = length.x > 0.0f ? length.x : length.x*(-1.0f);
	length.y = length.y > 0.0f ? length.y : length.y*(-1.0f);
	length.z = length.z > 0.0f ? length.z : length.z*(-1.0f);
	DirX = SVECTOR3(DirX.x / length.x, DirX.y / length.x, DirX.z / length.x);
	DirY = SVECTOR3(DirY.x / length.y, DirY.y / length.y, DirY.z / length.y);
	//#pragma omp parallel for
	uint32 weight_offset = size / 2;
	for (uint32 seek_w = 0; seek_w < size; seek_w = seek_w + 1)
	{
		//#pragma omp parallel for
		for (int32 seek = 0; seek < (int32) length.x; seek = seek + 1)
		{
			uint32 pos_x = (vec_begin.x - weight_offset + seek_w) + seek*DirX.x;
			uint32 pos_y = vec_begin.y + (DirX.y*seek);
			sfloat dis = vec_begin.z + DirX.z*seek;
			if ((pos_x < 0) || (pos_x >= buf_width) || (pos_y < 0) || (pos_y >= buf_height))
			{
				break;
			}
			pos_y = pos_y*buf_width * 4;
			pos_x = pos_x * 4;
			if (p_buffer[pos_y + pos_x + 3] >= dis)
			{
				p_buffer[pos_y + pos_x] = color.b;
				p_buffer[pos_y + pos_x + 1] = color.g;
				p_buffer[pos_y + pos_x + 2] = color.r;
				p_buffer[pos_y + pos_x + 3] = dis;
			}
		}
		//#pragma omp parallel for
		for (int32 seek = 0; seek < (int32) length.y; seek = seek + 1)
		{
			uint32 pos_x = (vec_begin.x - weight_offset + seek_w) + seek*DirY.x;
			uint32 pos_y = vec_begin.y + (seek*DirY.y);
			sfloat dis = vec_begin.z + DirY.z*seek;
			if ((pos_x < 0) || (pos_x >= buf_width) || (pos_y < 0) || (pos_y >= buf_height))
			{
				break;
			}
			pos_y = pos_y*buf_width * 4;
			pos_x = pos_x * 4;
			if (p_buffer[pos_y + pos_x + 3] >= dis)
			{
				p_buffer[pos_y + pos_x] = color.b;
				p_buffer[pos_y + pos_x + 1] = color.g;
				p_buffer[pos_y + pos_x + 2] = color.r;
				p_buffer[pos_y + pos_x + 3] = dis;
			}
		}
	}
}
void San::Graphics::DrawLineGraph(sfloat* p_buffer, const uint32 buf_width, const uint32 buf_height, const uint32 dest_width, const uint32 dest_height, sfloat* p_dataset, uint32 data_size, const SVECTOR3 pos, const SANCOLORF color, const uint32 size)
{
	if (p_buffer == nullptr)
	{
		return;
	}
	sfloat rate_x = (sfloat) data_size / (sfloat) dest_width;
	sfloat rate_y = dest_height;
	uint32 length = dest_width - 1;
	for (uint32 seek = 0; seek < length; seek = seek + 1)
	{
		uint32 index = ((sfloat) seek)*rate_x;
		SVECTOR3 pos_begin(seek, dest_height - p_dataset[index] * rate_y, 0.0f);
		index = ((sfloat) seek + 1.0f)*rate_x;
		SVECTOR3 pos_end(seek + 1, dest_height - p_dataset[index] * rate_y, 0.0f);
		pos_begin = pos_begin + pos;
		pos_end = pos_end + pos;
		DrawLineRGBA(p_buffer, buf_width, buf_height, pos_begin, pos_end, color, size);
	}
}