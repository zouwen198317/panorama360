#include "Api.h"

/************************************************************************/
/*				初始化拼接参数                                          */
/************************************************************************/
bool InitParam(StitchInfo *front, StitchInfo *rear, StitchInfo *left, StitchInfo *right)
{
	if (!front || !rear || !left || !right)
	{
		printf("p error\n");
		return false;
	}
	//初始化原图像
	front->srcImage = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, CHANNELS);
	rear->srcImage = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, CHANNELS);
	left->srcImage = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, CHANNELS);
	right->srcImage = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, CHANNELS);
	if (!front->srcImage || !rear->srcImage || !left->srcImage || !right->srcImage)
	{
		printf("create srcImage failed\n");
		return false;
	}
	cvZero(front->srcImage);
	cvZero(rear->srcImage);
	cvZero(left->srcImage);
	cvZero(right->srcImage);
	//初始化透视图
	front->undistorImage = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, CHANNELS);
	rear->undistorImage = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, CHANNELS);
	left->undistorImage = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, CHANNELS);
	right->undistorImage = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, CHANNELS);
	if (!front->undistorImage || !rear->undistorImage || !left->undistorImage || !right->undistorImage)
	{
		printf("create undistorImage failed\n");
		return false;
	}
	cvZero(front->undistorImage);
	cvZero(rear->undistorImage);
	cvZero(left->undistorImage);
	cvZero(right->undistorImage);
	//初始化俯视图
	front->birdImage = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, CHANNELS);
	rear->birdImage = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, CHANNELS);
	left->birdImage = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, CHANNELS);
	right->birdImage = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, CHANNELS);
	if (!front->birdImage || !rear->birdImage || !left->birdImage || !right->birdImage)
	{
		printf("create birdImage failed\n");
		return false;
	}
	cvZero(front->birdImage);
	cvZero(rear->birdImage);
	cvZero(left->birdImage);
	cvZero(right->birdImage);
	//初始化映射图
	front->transformImage = cvCreateImage(cvSize(STITCH_IMAGE_WIDTH, STITCH_IMAGE_HEIGHT), IPL_DEPTH_8U, CHANNELS);
	rear->transformImage = cvCreateImage(cvSize(STITCH_IMAGE_WIDTH, STITCH_IMAGE_HEIGHT), IPL_DEPTH_8U, CHANNELS);
	left->transformImage = cvCreateImage(cvSize(STITCH_IMAGE_WIDTH, STITCH_IMAGE_HEIGHT), IPL_DEPTH_8U, CHANNELS);
	right->transformImage = cvCreateImage(cvSize(STITCH_IMAGE_WIDTH, STITCH_IMAGE_HEIGHT), IPL_DEPTH_8U, CHANNELS);
	if (!front->transformImage || !rear->transformImage || !left->transformImage || !right->transformImage)
	{
		printf("create stitchImage failed\n");
		return false;
	}
	cvZero(front->transformImage);
	cvZero(rear->transformImage);
	cvZero(left->transformImage);
	cvZero(right->transformImage);	
	//初始化全景图
	front->stitchImage = cvCreateImage(cvSize(STITCH_IMAGE_WIDTH, STITCH_IMAGE_HEIGHT), IPL_DEPTH_8U, CHANNELS);
	if (!front->stitchImage)
	{
		printf("create stitchImage failed\n");
		return false;
	}
	cvZero(front->stitchImage);
	rear->stitchImage = front->stitchImage;
	left->stitchImage = front->stitchImage;
	right->stitchImage = front->stitchImage;
	//初始化畸变校正表
	GetUndistortMap(&front->mapx, &front->mapy, DIRECTION_FRONT);
	GetUndistortMap(&rear->mapx, &rear->mapy, DIRECTION_REAR);
	GetUndistortMap(&left->mapx, &left->mapy, DIRECTION_LEFT);
	GetUndistortMap(&right->mapx, &right->mapy, DIRECTION_RIGHT);
	//从文件读取棋盘格参数
	float tmpparam[4];
	if (!ReadBoardParam(tmpparam, FRONT_BOARD_PARAM_FILE_NAME))		//前向棋盘格参数
	{
		printf("read front board parameter faile\n");
		return false;
	}
	front->nboard_w = tmpparam[0];
	front->nboard_h = tmpparam[1];
	front->fboardwidth = tmpparam[2];
	front->fboardheight = tmpparam[3];
	if (!ReadBoardParam(tmpparam, REAR_BOARD_PARAM_FILE_NAME))		//后向棋盘格参数
	{
		printf("read rear board parameter faile\n");
		return false;
	}
	rear->nboard_w = tmpparam[0];
	rear->nboard_h = tmpparam[1];
	rear->fboardwidth = tmpparam[2];
	rear->fboardheight = tmpparam[3];
	if (!ReadBoardParam(tmpparam, LEFT_BOARD_PARAM_FILE_NAME))		//左向棋盘格参数
	{
		printf("read board parameter faile\n");
		return false;
	}
	left->nboard_w = tmpparam[0];
	left->nboard_h = tmpparam[1];
	left->fboardwidth = tmpparam[2];
	left->fboardheight = tmpparam[3];
	if (!ReadBoardParam(tmpparam, RIGHT_BOARD_PARAM_FILE_NAME))		//右向棋盘格参数
	{
		printf("read board parameter faile\n");
		return false;
	}
	right->nboard_w = tmpparam[0];
	right->nboard_h = tmpparam[1];
	right->fboardwidth = tmpparam[2];
	right->fboardheight = tmpparam[3];
	//初始化俯视图中标记点坐标
	front->srcMarkPoint		= new CvPoint2D32f[4];
	front->birdMarkPoint	= new CvPoint2D32f[4];
	front->stitchMarkPoint	= new CvPoint2D32f[4];
	rear->srcMarkPoint		= new CvPoint2D32f[4];
	rear->birdMarkPoint		= new CvPoint2D32f[4];
	rear->stitchMarkPoint	= new CvPoint2D32f[4];
	left->srcMarkPoint		= new CvPoint2D32f[4];
	left->birdMarkPoint		= new CvPoint2D32f[4];
	left->stitchMarkPoint	= new CvPoint2D32f[4];
	right->srcMarkPoint		= new CvPoint2D32f[4];
	right->birdMarkPoint	= new CvPoint2D32f[4];
	right->stitchMarkPoint	= new CvPoint2D32f[4];

	//初始化棋盘点坐标
	front->srcCorners = new CvPoint2D32f[front->nboard_w*front->nboard_h];
	front->birdCorners = new CvPoint2D32f[front->nboard_w*front->nboard_h];
	rear->srcCorners = new CvPoint2D32f[rear->nboard_w*rear->nboard_h];
	rear->birdCorners = new CvPoint2D32f[rear->nboard_w*rear->nboard_h];
	left->srcCorners = new CvPoint2D32f[left->nboard_w*left->nboard_h];
	left->birdCorners = new CvPoint2D32f[left->nboard_w*left->nboard_h];
	right->srcCorners = new CvPoint2D32f[right->nboard_w*right->nboard_h];
	right->birdCorners = new CvPoint2D32f[right->nboard_w*right->nboard_h];
	//初始化stitchPoint
	front->stitchPoint = new CvPoint2D32f[6];
	rear->stitchPoint  = new CvPoint2D32f[6];
	left->stitchPoint  = new CvPoint2D32f[6];
	right->stitchPoint = new CvPoint2D32f[6];
	//初始化所以矩阵
	front->birdMat = cvCreateMat(3, 3, CV_64FC1); 
	front->stitchMat = cvCreateMat(3, 3, CV_64FC1); 
	rear->birdMat = cvCreateMat(3, 3, CV_64FC1); 
	rear->stitchMat = cvCreateMat(3, 3, CV_64FC1); 
	left->birdMat = cvCreateMat(3, 3, CV_64FC1); 
	left->stitchMat = cvCreateMat(3, 3, CV_64FC1); 
	right->birdMat = cvCreateMat(3, 3, CV_64FC1); 
	right->stitchMat = cvCreateMat(3, 3, CV_64FC1); 

	return true;
}
/************************************************************************/
/*				获取俯视图                                              */
/************************************************************************/
bool GetBirdImage(StitchInfo *stInfo)
{
	if (!stInfo)
	{
		printf("p error\n");
		return false;
	}
	//畸变校正
	if(!RemapImage(stInfo->srcImage, stInfo->undistorImage, stInfo->mapx, stInfo->mapy))
	{
		printf("remap image failed\n");
		return false;
	}
	//cvCopy(stInfo->srcImage, stInfo->undistorImage);
	//获取俯视图
	if (!FindBirdImage(stInfo->undistorImage, stInfo->birdImage, stInfo->nboard_w, stInfo->nboard_h, 
		stInfo->srcCorners, stInfo->birdMat, SCALE))
	{
		printf("get birdImage failed\n");
		return false;
	}
	TransformPoint(stInfo->srcCorners, stInfo->nboard_w*stInfo->nboard_h, stInfo->birdMat, stInfo->birdCorners);

	return true;
}
/************************************************************************/
/*				计算标记点                                              */
/************************************************************************/
bool GetMarkPoint(StitchInfo *front, StitchInfo *rear, StitchInfo *left, StitchInfo *right)
{
	if (!front || !rear || !left || !right)
	{
		return false;
	}
	if (!ReadMarkpoint(front->srcMarkPoint, rear->srcMarkPoint, left->srcMarkPoint, right->srcMarkPoint, MARKPOINT_FILE_NAME))
	{
		printf("read markpoint failed\n");
		return false;
	}
	//计算俯视图中标记点坐标
	CalculateMarkpoint(front->undistorImage, front->birdImage, front->birdCorners, front->srcMarkPoint, front->birdMarkPoint, front->nboard_w, 
		front->nboard_h, front->fboardwidth, front->fboardheight, front->birdMat);
	CalculateMarkpoint(rear->undistorImage, rear->birdImage, rear->birdCorners, rear->srcMarkPoint, rear->birdMarkPoint, rear->nboard_w, 
		rear->nboard_h, rear->fboardwidth, rear->fboardheight, rear->birdMat);
	CalculateMarkpoint(left->undistorImage, left->birdImage, left->birdCorners, left->srcMarkPoint, left->birdMarkPoint, left->nboard_w, 
		left->nboard_h, left->fboardwidth, left->fboardheight, left->birdMat);
	CalculateMarkpoint(right->undistorImage, right->birdImage, right->birdCorners, right->srcMarkPoint, right->birdMarkPoint, right->nboard_w, 
		right->nboard_h, right->fboardwidth, right->fboardheight, right->birdMat);
	//for (int i = 0; i < 4; ++i)
	//{
	//	drawpoint(front->birdImage, cvPoint(front->birdMarkPoint[i].x, front->birdMarkPoint[i].y));
	//	drawpoint(rear->birdImage, cvPoint(rear->birdMarkPoint[i].x, rear->birdMarkPoint[i].y));
	//	drawpoint(left->birdImage, cvPoint(left->birdMarkPoint[i].x, left->birdMarkPoint[i].y));
	//	drawpoint(right->birdImage, cvPoint(right->birdMarkPoint[i].x, right->birdMarkPoint[i].y));
	//}
	return true;
}
/************************************************************************/
/*              拼接                                                     */
/************************************************************************/
bool StitchImage(StitchInfo *front, StitchInfo *rear, StitchInfo *left, StitchInfo *right)
{
	if (!front || !rear || !left || !right)
	{
		return false;
	}
	//计算映射到全景图的映射矩阵
	GetStitchMat(front->birdImage, rear->birdImage, left->birdImage, right->birdImage, front->birdMarkPoint, rear->birdMarkPoint, 
		left->birdMarkPoint, right->birdMarkPoint, &front->stitchMat, &rear->stitchMat, &left->stitchMat, &right->stitchMat, front->stitchImage);
	//计算全景图中各拼接缝的两端点坐标
	CvPoint2D32f tmppoint;
	TransformPoint(front->birdMarkPoint, 4, front->stitchMat, front->stitchMarkPoint);
	TransformPoint(rear->birdMarkPoint, 4, rear->stitchMat, rear->stitchMarkPoint);
	TransformPoint(left->birdMarkPoint, 4, left->stitchMat, left->stitchMarkPoint);
	TransformPoint(right->birdMarkPoint, 4, right->stitchMat, right->stitchMarkPoint);
	//front
	front->stitchPoint[0].x = 0;
	front->stitchPoint[0].y = front->stitchMarkPoint[1].y-front->stitchMarkPoint[1].x*
		((front->stitchMarkPoint[1].y-front->stitchMarkPoint[0].y)/(front->stitchMarkPoint[1].x-front->stitchMarkPoint[0].x));	
	front->stitchPoint[1] = cvPoint2D32f(0, 0);
	front->stitchPoint[2] = cvPoint2D32f(front->stitchImage->width, 0);
	front->stitchPoint[3].x = front->stitchImage->width;
	front->stitchPoint[3].y = front->stitchMarkPoint[2].y+(front->stitchImage->width-front->stitchMarkPoint[2].x)*
		(front->stitchMarkPoint[3].y-front->stitchMarkPoint[2].y)/(front->stitchMarkPoint[3].x-front->stitchMarkPoint[2].x);
	tmppoint.x = front->birdMarkPoint[2].x+(front->birdImage->height-front->birdMarkPoint[2].y)*
		(front->birdMarkPoint[3].x-front->birdMarkPoint[2].x)/(front->birdMarkPoint[3].y-front->birdMarkPoint[2].y);
	tmppoint.y = front->birdImage->height;
	TransformPoint(&tmppoint, 1, front->stitchMat, &front->stitchPoint[4]);
	tmppoint.x = front->birdMarkPoint[0].x+(front->birdImage->height-front->birdMarkPoint[0].y)*
		(front->birdMarkPoint[1].x-front->birdMarkPoint[0].x)/(front->birdMarkPoint[1].y-front->birdMarkPoint[0].y);
	tmppoint.y = front->birdImage->height;
	TransformPoint(&tmppoint, 1, front->stitchMat, &front->stitchPoint[5]);
	//rear
	rear->stitchPoint[0].x = rear->stitchImage->width;
	rear->stitchPoint[0].y = rear->stitchMarkPoint[0].y+(rear->stitchImage->width-rear->stitchMarkPoint[0].x)*
		(rear->stitchMarkPoint[1].y-rear->stitchMarkPoint[0].y)/(rear->stitchMarkPoint[1].x-rear->stitchMarkPoint[0].x);	
	rear->stitchPoint[1] = cvPoint2D32f(rear->stitchImage->width, rear->stitchImage->height);
	rear->stitchPoint[2] = cvPoint2D32f(0, rear->stitchImage->height);
	rear->stitchPoint[3].x = 0;
	rear->stitchPoint[3].y = rear->stitchMarkPoint[2].y-rear->stitchMarkPoint[2].x*
		(rear->stitchMarkPoint[3].y-rear->stitchMarkPoint[2].y)/(rear->stitchMarkPoint[3].x-rear->stitchMarkPoint[2].x);
	tmppoint.x = rear->birdMarkPoint[2].x+(rear->birdImage->height-rear->birdMarkPoint[2].y)*
		(rear->birdMarkPoint[3].x-rear->birdMarkPoint[2].x)/(rear->birdMarkPoint[3].y-rear->birdMarkPoint[2].y);
	tmppoint.y = rear->birdImage->height;
	TransformPoint(&tmppoint, 1, rear->stitchMat, &rear->stitchPoint[4]);
	tmppoint.x = rear->birdMarkPoint[0].x+(rear->birdImage->height-rear->birdMarkPoint[0].y)*
		(rear->birdMarkPoint[1].x-rear->birdMarkPoint[0].x)/(rear->birdMarkPoint[1].y-rear->birdMarkPoint[0].y);
	tmppoint.y = rear->birdImage->height;
	TransformPoint(&tmppoint, 1, rear->stitchMat, &rear->stitchPoint[5]);
	//left
	left->stitchPoint[0].x = rear->stitchPoint[3].x;
	left->stitchPoint[0].y = rear->stitchPoint[3].y;
	left->stitchPoint[1] = cvPoint2D32f(left->stitchPoint[0].x, left->stitchPoint[0].y);
	left->stitchPoint[2].x = front->stitchPoint[0].x;
	left->stitchPoint[2].y = front->stitchPoint[0].y;
	left->stitchPoint[3] = cvPoint2D32f(left->stitchPoint[2].x, left->stitchPoint[2].y);
	tmppoint.x = left->birdMarkPoint[2].x+(left->birdImage->height-left->birdMarkPoint[2].y)*
		(left->birdMarkPoint[3].x-left->birdMarkPoint[2].x)/(left->birdMarkPoint[3].y-left->birdMarkPoint[2].y);
	tmppoint.y = left->birdImage->height;
	TransformPoint(&tmppoint, 1, left->stitchMat, &left->stitchPoint[4]);
	tmppoint.x = left->birdMarkPoint[0].x+(left->birdImage->height-left->birdMarkPoint[0].y)*
		(left->birdMarkPoint[1].x-left->birdMarkPoint[0].x)/(left->birdMarkPoint[1].y-left->birdMarkPoint[0].y);
	tmppoint.y = left->birdImage->height;
	TransformPoint(&tmppoint, 1, left->stitchMat, &left->stitchPoint[5]);
	//right
	right->stitchPoint[0].x = front->stitchPoint[3].x;
	right->stitchPoint[0].y = front->stitchPoint[3].y;
	right->stitchPoint[1]	= cvPoint2D32f(right->stitchPoint[0].x, right->stitchPoint[0].y);
	right->stitchPoint[2].x = rear->stitchPoint[0].x;
	right->stitchPoint[2].y = rear->stitchPoint[0].y;
	right->stitchPoint[3]	= cvPoint2D32f(right->stitchPoint[2].x, right->stitchPoint[2].y);
	tmppoint.x				= right->birdMarkPoint[2].x+(right->birdImage->height-right->birdMarkPoint[2].y)*
		(right->birdMarkPoint[3].x-right->birdMarkPoint[2].x)/(right->birdMarkPoint[3].y-right->birdMarkPoint[2].y);
	tmppoint.y				= right->birdImage->height;
	TransformPoint(&tmppoint, 1, right->stitchMat, &right->stitchPoint[4]);
	tmppoint.x				= right->birdMarkPoint[0].x+(right->birdImage->height-right->birdMarkPoint[0].y)*
		(right->birdMarkPoint[1].x-right->birdMarkPoint[0].x)/(right->birdMarkPoint[1].y-right->birdMarkPoint[0].y);
	tmppoint.y				= right->birdImage->height;
	TransformPoint(&tmppoint, 1, right->stitchMat, &right->stitchPoint[5]);


	//裁剪
	//cutImage(front->birdImage, front->birdMarkPoint[0], front->birdMarkPoint[1], CUT_LEFT_BOTTOM);
	//cutImage(front->birdImage, front->birdMarkPoint[2], front->birdMarkPoint[3], CUT_RIGHT_BOTTOM);
	//cutImage(rear->birdImage, rear->birdMarkPoint[0], rear->birdMarkPoint[1], CUT_LEFT_BOTTOM);
	//cutImage(rear->birdImage, rear->birdMarkPoint[2], rear->birdMarkPoint[3], CUT_RIGHT_BOTTOM);
	//cutImage(left->birdImage, left->birdMarkPoint[0], left->birdMarkPoint[1], CUT_LEFT_BOTTOM);
	//cutImage(left->birdImage, left->birdMarkPoint[2], left->birdMarkPoint[3], CUT_RIGHT_BOTTOM);
	//cutImage(right->birdImage, right->birdMarkPoint[0], right->birdMarkPoint[1], CUT_LEFT_BOTTOM);
	//cutImage(right->birdImage, right->birdMarkPoint[2], right->birdMarkPoint[3], CUT_RIGHT_BOTTOM);

	//映射到拼接图
	cvWarpPerspective(front->birdImage, front->transformImage, front->stitchMat, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS , cvScalarAll(0));
	cvWarpPerspective(rear->birdImage, rear->transformImage, rear->stitchMat, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS , cvScalarAll(0));
	cvWarpPerspective(left->birdImage, left->transformImage, left->stitchMat, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS , cvScalarAll(0));
	cvWarpPerspective(right->birdImage, right->transformImage, right->stitchMat, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS , cvScalarAll(0));

	//cvAdd(front->transformImage, front->stitchImage, front->stitchImage);
	//cvAdd(rear->transformImage, front->stitchImage, front->stitchImage);
	//cvAdd(left->transformImage, front->stitchImage, front->stitchImage);
	//cvAdd(right->transformImage, front->stitchImage, front->stitchImage);
	//cvSaveImage("简单拼接.jpg", front->stitchImage);
	//融合
	BlendImage(front, rear, left, right);
	cvSaveImage("融合拼接.jpg", front->stitchImage);

	return true;
}
/************************************************************************/
/*				输出全景表                                              */
/************************************************************************/
bool WriteStitchMap(StitchInfo *front, StitchInfo *rear, StitchInfo *left, StitchInfo *right)
{
	if (!front || !rear || !left || !right)
	{
		return false;
	}
	CvMat *frontIntrinsics	= (CvMat*)cvLoad(INTRINSICS_FILE_NAME_FRONT);
	CvMat *frontDistortion	= (CvMat*)cvLoad(DISTORTION_FILE_NAME_FRONT);
	CvMat *rearIntrinsics	= (CvMat*)cvLoad(INTRINSICS_FILE_NAME_REAR);
	CvMat *rearDistortion	= (CvMat*)cvLoad(DISTORTION_FILE_NAME_REAR);
	CvMat *leftIntrinsics	= (CvMat*)cvLoad(INTRINSICS_FILE_NAME_LEFT);
	CvMat *leftDistortion	= (CvMat*)cvLoad(DISTORTION_FILE_NAME_LEFT);
	CvMat *rightIntrinsics	= (CvMat*)cvLoad(INTRINSICS_FILE_NAME_RIGHT);
	CvMat *rightDistortion	= (CvMat*)cvLoad(DISTORTION_FILE_NAME_RIGHT);
	if (!frontIntrinsics || !frontDistortion || !leftIntrinsics || !leftDistortion || 
		!rearIntrinsics || !rearDistortion || !rightIntrinsics || !rightDistortion)
	{
		printf("read lens mat param failed\n");
		return false;
	}
	double frontfx = CV_MAT_ELEM(*frontIntrinsics, float, 0, 0);
	double frontfy = CV_MAT_ELEM(*frontIntrinsics, float, 1, 1);
	double frontcx = CV_MAT_ELEM(*frontIntrinsics, float, 0, 2);
	double frontcy = CV_MAT_ELEM(*frontIntrinsics, float, 1, 2);
	double frontk1 = CV_MAT_ELEM(*frontDistortion, float, 0, 0);
	double frontk2 = CV_MAT_ELEM(*frontDistortion, float, 1, 0);
	double frontk3 = CV_MAT_ELEM(*frontDistortion, float, 2, 0);
	double frontp1 = CV_MAT_ELEM(*frontDistortion, float, 3, 0);
	double frontp2 = CV_MAT_ELEM(*frontDistortion, float, 4, 0);

	double rearfx = CV_MAT_ELEM(*rearIntrinsics, float, 0, 0);
	double rearfy = CV_MAT_ELEM(*rearIntrinsics, float, 1, 1);
	double rearcx = CV_MAT_ELEM(*rearIntrinsics, float, 0, 2);
	double rearcy = CV_MAT_ELEM(*rearIntrinsics, float, 1, 2);
	double reark1 = CV_MAT_ELEM(*rearDistortion, float, 0, 0);
	double reark2 = CV_MAT_ELEM(*rearDistortion, float, 1, 0);
	double reark3 = CV_MAT_ELEM(*rearDistortion, float, 2, 0);
	double rearp1 = CV_MAT_ELEM(*rearDistortion, float, 3, 0);
	double rearp2 = CV_MAT_ELEM(*rearDistortion, float, 4, 0);

	double leftfx = CV_MAT_ELEM(*leftIntrinsics, float, 0, 0);
	double leftfy = CV_MAT_ELEM(*leftIntrinsics, float, 1, 1);
	double leftcx = CV_MAT_ELEM(*leftIntrinsics, float, 0, 2);
	double leftcy = CV_MAT_ELEM(*leftIntrinsics, float, 1, 2);
	double leftk1 = CV_MAT_ELEM(*leftDistortion, float, 0, 0);
	double leftk2 = CV_MAT_ELEM(*leftDistortion, float, 1, 0);
	double leftk3 = CV_MAT_ELEM(*leftDistortion, float, 2, 0);
	double leftp1 = CV_MAT_ELEM(*leftDistortion, float, 3, 0);
	double leftp2 = CV_MAT_ELEM(*leftDistortion, float, 4, 0);

	double rightfx = CV_MAT_ELEM(*rightIntrinsics, float, 0, 0);
	double rightfy = CV_MAT_ELEM(*rightIntrinsics, float, 1, 1);
	double rightcx = CV_MAT_ELEM(*rightIntrinsics, float, 0, 2);
	double rightcy = CV_MAT_ELEM(*rightIntrinsics, float, 1, 2);
	double rightk1 = CV_MAT_ELEM(*rightDistortion, float, 0, 0);
	double rightk2 = CV_MAT_ELEM(*rightDistortion, float, 1, 0);
	double rightk3 = CV_MAT_ELEM(*rightDistortion, float, 2, 0);
	double rightp1 = CV_MAT_ELEM(*rightDistortion, float, 3, 0);
	double rightp2 = CV_MAT_ELEM(*rightDistortion, float, 4, 0);

	CvMat *frontcoordinatexMat	= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *frontcoordinateyMat	= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *frontweightMat		= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *rearcoordinatexMat	= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *rearcoordinateyMat	= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *rearweightMat		= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *leftcoordinatexMat	= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *leftcoordinateyMat	= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *leftweightMat		= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *rightcoordinatexMat	= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *rightcoordinateyMat	= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *rightweightMat		= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);

	CvMat *invertFrontStitchMat = cvCreateMat(3, 3, CV_64FC1); 
	CvMat *invertRearStitchMat = cvCreateMat(3, 3, CV_64FC1); 
	CvMat *invertLeftStitchMat = cvCreateMat(3, 3, CV_64FC1); 
	CvMat *invertRightStitchMat = cvCreateMat(3, 3, CV_64FC1); 
	CvMat *invertFrontBirdMat = cvCreateMat(3, 3, CV_64FC1); 
	CvMat *invertRearBirdMat = cvCreateMat(3, 3, CV_64FC1); 
	CvMat *invertLeftBirdMat = cvCreateMat(3, 3, CV_64FC1); 
	CvMat *invertRightBirdMat = cvCreateMat(3, 3, CV_64FC1); 
	cvInvert(front->birdMat, invertFrontBirdMat, CV_SVD);
	cvInvert(front->stitchMat, invertFrontStitchMat, CV_SVD);
	cvInvert(rear->birdMat, invertRearBirdMat, CV_SVD);
	cvInvert(rear->stitchMat, invertRearStitchMat, CV_SVD);
	cvInvert(left->birdMat, invertLeftBirdMat, CV_SVD);
	cvInvert(left->stitchMat, invertLeftStitchMat, CV_SVD);
	cvInvert(right->birdMat, invertRightBirdMat, CV_SVD);
	cvInvert(right->stitchMat, invertRightStitchMat, CV_SVD);
	CvPoint2D32f tmppoint;
	printf("start writting map\n");
	for (int x = 0; x < STITCH_IMAGE_WIDTH; ++x)
	{
		printf("x=%d\n", x);
		for (int y = 0; y < STITCH_IMAGE_HEIGHT; ++y)
		{
			if (JudgeIsInside(cvPoint(x, y), front->stitchPoint, 6))
			{
				//前向
				tmppoint = cvPoint2D32f(x, y);
				TransformPoint(&tmppoint, 1, invertFrontStitchMat, &tmppoint);
				TransformPoint(&tmppoint, 1, invertFrontBirdMat, &tmppoint);
				GetDistorPoint(&tmppoint, 1, &tmppoint, frontfx, frontfy, frontcx, frontcy, frontk1, frontk2, frontk3, frontp1, frontp2);
				CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)	= tmppoint.x;
				CV_MAT_ELEM(*frontcoordinateyMat, float, y, x)	= tmppoint.y;
				CV_MAT_ELEM(*frontweightMat, float, y, x)		= 1;
				//后向
				CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rearcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rearweightMat, float, y, x)		= 0;
				//左向
				CV_MAT_ELEM(*leftcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*leftcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*leftweightMat, float, y, x)		= 0;
				//右向
				CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rightweightMat, float, y, x)		= 0;
			}else if (JudgeIsInside(cvPoint(x, y), rear->stitchPoint, 6))
			{
				//前向
				CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*frontcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*frontweightMat, float, y, x)		= 0;
				//后向
				tmppoint = cvPoint2D32f(x, y);
				TransformPoint(&tmppoint, 1, invertRearStitchMat, &tmppoint);
				TransformPoint(&tmppoint, 1, invertRearBirdMat, &tmppoint);
				GetDistorPoint(&tmppoint, 1, &tmppoint, rearfx, rearfy, rearcx, rearcy, reark1, reark2, reark3, rearp1, rearp2);
				CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)	= tmppoint.x;
				CV_MAT_ELEM(*rearcoordinateyMat, float, y, x)	= tmppoint.y;
				CV_MAT_ELEM(*rearweightMat, float, y, x)		= 1;				
				//左向
				CV_MAT_ELEM(*leftcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*leftcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*leftweightMat, float, y, x)		= 0;
				//右向
				CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rightweightMat, float, y, x)		= 0;
			}else if (JudgeIsInside(cvPoint(x, y), left->stitchPoint, 6))
			{
				//前向
				CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*frontcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*frontweightMat, float, y, x)		= 0;
				//后向
				CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rearcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rearweightMat, float, y, x)		= 0;
				//左向
				tmppoint = cvPoint2D32f(x, y);
				TransformPoint(&tmppoint, 1, invertLeftStitchMat, &tmppoint);
				TransformPoint(&tmppoint, 1, invertLeftBirdMat, &tmppoint);
				GetDistorPoint(&tmppoint, 1, &tmppoint, leftfx, leftfy, leftcx, leftcy, leftk1, leftk2, leftk3, leftp1, leftp2);
				CV_MAT_ELEM(*leftcoordinatexMat, float, y, x) = tmppoint.x;
				CV_MAT_ELEM(*leftcoordinateyMat, float, y, x) = tmppoint.y;
				CV_MAT_ELEM(*leftweightMat, float, y, x) = 1;
				//右向
				CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rightweightMat, float, y, x)		= 0;
			}else if (JudgeIsInside(cvPoint(x, y), right->stitchPoint, 6))
			{
				//前向
				CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*frontcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*frontweightMat, float, y, x)		= 0;
				//后向
				CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rearcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rearweightMat, float, y, x)		= 0;
				//左向
				CV_MAT_ELEM(*leftcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*leftcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*leftweightMat, float, y, x)		= 0;
				//右向
				tmppoint = cvPoint2D32f(x, y);
				TransformPoint(&tmppoint, 1, invertRightStitchMat, &tmppoint);
				TransformPoint(&tmppoint, 1, invertRightBirdMat, &tmppoint);
				GetDistorPoint(&tmppoint, 1, &tmppoint, rightfx, rightfy, rightcx, rightcy, rightk1, rightk2, rightk3, rightp1, rightp2);
				CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)	= tmppoint.x;
				CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)	= tmppoint.y;
				CV_MAT_ELEM(*rightweightMat, float, y, x)		= 1;
			}else
			{
				//前向
				CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*frontcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*frontweightMat, float, y, x)		= 0;
				//后向
				CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rearcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rearweightMat, float, y, x)	= 0;
				//左向
				CV_MAT_ELEM(*leftcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*leftcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*leftweightMat, float, y, x)		= 0;
				//右向
				CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rightweightMat, float, y, x)		= 0;
			}
		}
	}
	printf("stop writting map\n");
	cvSave(FRONT_COORDINATE_X_FILE_NAME, frontcoordinatexMat);
	cvSave(FRONT_COORDINATE_Y_FILE_NAME, frontcoordinateyMat);
	cvSave(FRONT_WEIGHT_FILE_NAME, frontweightMat);
	cvSave(REAR_COORDINATE_X_FILE_NAME, rearcoordinatexMat);
	cvSave(REAR_COORDINATE_Y_FILE_NAME, rearcoordinateyMat);
	cvSave(REAR_WEIGHT_FILE_NAME, rearweightMat);
	cvSave(LEFT_COORDINATE_X_FILE_NAME, leftcoordinatexMat);
	cvSave(LEFT_COORDINATE_Y_FILE_NAME, leftcoordinateyMat);
	cvSave(LEFT_WEIGHT_FILE_NAME, leftweightMat);
	cvSave(RIGHT_COORDINATE_X_FILE_NAME, rightcoordinatexMat);
	cvSave(RIGHT_COORDINATE_Y_FILE_NAME, rightcoordinateyMat);
	cvSave(RIGHT_WEIGHT_FILE_NAME, rightweightMat);

	cvReleaseMat(&frontcoordinatexMat);
	cvReleaseMat(&frontcoordinateyMat);
	cvReleaseMat(&frontweightMat);
	cvReleaseMat(&rearcoordinatexMat);
	cvReleaseMat(&rearcoordinateyMat);
	cvReleaseMat(&rearweightMat);
	cvReleaseMat(&leftcoordinatexMat);
	cvReleaseMat(&leftcoordinateyMat);
	cvReleaseMat(&leftweightMat);
	cvReleaseMat(&rightcoordinatexMat);
	cvReleaseMat(&rightcoordinateyMat);
	cvReleaseMat(&rightweightMat);
	cvReleaseMat(&invertFrontBirdMat);
	cvReleaseMat(&invertFrontStitchMat);
	cvReleaseMat(&invertRearBirdMat);
	cvReleaseMat(&invertRearStitchMat);
	cvReleaseMat(&invertLeftBirdMat);
	cvReleaseMat(&invertLeftStitchMat);
	cvReleaseMat(&invertRightBirdMat);
	cvReleaseMat(&invertRightStitchMat);
	return true;
}
/************************************************************************/
/*				输出全景表(带权值）                                     */
/************************************************************************/
bool WriteStitchMap2(StitchInfo *front, StitchInfo *rear, StitchInfo *left, StitchInfo *right)
{
	if (!front || !rear || !left || !right)
	{
		return false;
	}
	CvMat *frontIntrinsics	= (CvMat*)cvLoad(INTRINSICS_FILE_NAME_FRONT);
	CvMat *frontDistortion	= (CvMat*)cvLoad(DISTORTION_FILE_NAME_FRONT);
	CvMat *rearIntrinsics	= (CvMat*)cvLoad(INTRINSICS_FILE_NAME_REAR);
	CvMat *rearDistortion	= (CvMat*)cvLoad(DISTORTION_FILE_NAME_REAR);
	CvMat *leftIntrinsics	= (CvMat*)cvLoad(INTRINSICS_FILE_NAME_LEFT);
	CvMat *leftDistortion	= (CvMat*)cvLoad(DISTORTION_FILE_NAME_LEFT);
	CvMat *rightIntrinsics	= (CvMat*)cvLoad(INTRINSICS_FILE_NAME_RIGHT);
	CvMat *rightDistortion	= (CvMat*)cvLoad(DISTORTION_FILE_NAME_RIGHT);
	if (!frontIntrinsics || !frontDistortion || !leftIntrinsics || !leftDistortion || 
		!rearIntrinsics || !rearDistortion || !rightIntrinsics || !rightDistortion)
	{
		printf("read lens mat param failed\n");
		return false;
	}
	double frontfx = CV_MAT_ELEM(*frontIntrinsics, float, 0, 0);
	double frontfy = CV_MAT_ELEM(*frontIntrinsics, float, 1, 1);
	double frontcx = CV_MAT_ELEM(*frontIntrinsics, float, 0, 2);
	double frontcy = CV_MAT_ELEM(*frontIntrinsics, float, 1, 2);
	double frontk1 = CV_MAT_ELEM(*frontDistortion, float, 0, 0);
	double frontk2 = CV_MAT_ELEM(*frontDistortion, float, 1, 0);
	double frontk3 = CV_MAT_ELEM(*frontDistortion, float, 2, 0);
	double frontp1 = CV_MAT_ELEM(*frontDistortion, float, 3, 0);
	double frontp2 = CV_MAT_ELEM(*frontDistortion, float, 4, 0);

	double rearfx = CV_MAT_ELEM(*rearIntrinsics, float, 0, 0);
	double rearfy = CV_MAT_ELEM(*rearIntrinsics, float, 1, 1);
	double rearcx = CV_MAT_ELEM(*rearIntrinsics, float, 0, 2);
	double rearcy = CV_MAT_ELEM(*rearIntrinsics, float, 1, 2);
	double reark1 = CV_MAT_ELEM(*rearDistortion, float, 0, 0);
	double reark2 = CV_MAT_ELEM(*rearDistortion, float, 1, 0);
	double reark3 = CV_MAT_ELEM(*rearDistortion, float, 2, 0);
	double rearp1 = CV_MAT_ELEM(*rearDistortion, float, 3, 0);
	double rearp2 = CV_MAT_ELEM(*rearDistortion, float, 4, 0);

	double leftfx = CV_MAT_ELEM(*leftIntrinsics, float, 0, 0);
	double leftfy = CV_MAT_ELEM(*leftIntrinsics, float, 1, 1);
	double leftcx = CV_MAT_ELEM(*leftIntrinsics, float, 0, 2);
	double leftcy = CV_MAT_ELEM(*leftIntrinsics, float, 1, 2);
	double leftk1 = CV_MAT_ELEM(*leftDistortion, float, 0, 0);
	double leftk2 = CV_MAT_ELEM(*leftDistortion, float, 1, 0);
	double leftk3 = CV_MAT_ELEM(*leftDistortion, float, 2, 0);
	double leftp1 = CV_MAT_ELEM(*leftDistortion, float, 3, 0);
	double leftp2 = CV_MAT_ELEM(*leftDistortion, float, 4, 0);

	double rightfx = CV_MAT_ELEM(*rightIntrinsics, float, 0, 0);
	double rightfy = CV_MAT_ELEM(*rightIntrinsics, float, 1, 1);
	double rightcx = CV_MAT_ELEM(*rightIntrinsics, float, 0, 2);
	double rightcy = CV_MAT_ELEM(*rightIntrinsics, float, 1, 2);
	double rightk1 = CV_MAT_ELEM(*rightDistortion, float, 0, 0);
	double rightk2 = CV_MAT_ELEM(*rightDistortion, float, 1, 0);
	double rightk3 = CV_MAT_ELEM(*rightDistortion, float, 2, 0);
	double rightp1 = CV_MAT_ELEM(*rightDistortion, float, 3, 0);
	double rightp2 = CV_MAT_ELEM(*rightDistortion, float, 4, 0);

	CvMat *frontcoordinatexMat	= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *frontcoordinateyMat	= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *frontweightMat		= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *rearcoordinatexMat	= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *rearcoordinateyMat	= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *rearweightMat		= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *leftcoordinatexMat	= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *leftcoordinateyMat	= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *leftweightMat		= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *rightcoordinatexMat	= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *rightcoordinateyMat	= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);
	CvMat *rightweightMat		= cvCreateMat(STITCH_IMAGE_HEIGHT, STITCH_IMAGE_WIDTH, CV_32FC1);

	CvMat *invertFrontStitchMat = cvCreateMat(3, 3, CV_64FC1); 
	CvMat *invertRearStitchMat = cvCreateMat(3, 3, CV_64FC1); 
	CvMat *invertLeftStitchMat = cvCreateMat(3, 3, CV_64FC1); 
	CvMat *invertRightStitchMat = cvCreateMat(3, 3, CV_64FC1); 
	CvMat *invertFrontBirdMat = cvCreateMat(3, 3, CV_64FC1); 
	CvMat *invertRearBirdMat = cvCreateMat(3, 3, CV_64FC1); 
	CvMat *invertLeftBirdMat = cvCreateMat(3, 3, CV_64FC1); 
	CvMat *invertRightBirdMat = cvCreateMat(3, 3, CV_64FC1); 
	cvInvert(front->birdMat, invertFrontBirdMat, CV_SVD);
	cvInvert(front->stitchMat, invertFrontStitchMat, CV_SVD);
	cvInvert(rear->birdMat, invertRearBirdMat, CV_SVD);
	cvInvert(rear->stitchMat, invertRearStitchMat, CV_SVD);
	cvInvert(left->birdMat, invertLeftBirdMat, CV_SVD);
	cvInvert(left->stitchMat, invertLeftStitchMat, CV_SVD);
	cvInvert(right->birdMat, invertRightBirdMat, CV_SVD);
	cvInvert(right->stitchMat, invertRightStitchMat, CV_SVD);
	CvPoint2D32f tmppoint;
	float A1, B1, C1, AABB1, D1;
	float A2, B2, C2, AABB2, D2;

#define MaxBlend 5
	printf("start writting map\n");
	for (int x = 0; x < STITCH_IMAGE_WIDTH; ++x)
	{
		for (int y = 0; y < STITCH_IMAGE_HEIGHT; ++y)
		{
			if (JudgeIsInside(cvPoint(x, y), front->stitchPoint, 6))
			{
				A1 = front->stitchPoint[5].y-front->stitchPoint[0].y;
				B1 = front->stitchPoint[0].x-front->stitchPoint[5].x;
				C1 = front->stitchPoint[5].x*front->stitchPoint[0].y-front->stitchPoint[0].x*front->stitchPoint[5].y;
				AABB1 = sqrt(A1*A1+B1*B1);
				D1 = abs(A1*x+B1*y+C1)/AABB1;
				A2 = front->stitchPoint[4].y-front->stitchPoint[3].y;
				B2 = front->stitchPoint[3].x-front->stitchPoint[4].x;
				C2 = front->stitchPoint[4].x*front->stitchPoint[3].y-front->stitchPoint[3].x*front->stitchPoint[4].y;
				AABB2 = sqrt(A2*A2+B2*B2);
				D2 = abs(A2*x+B2*y+C2)/AABB2;
				if (D1 < MaxBlend)
				{
					//前向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertFrontStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertFrontBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, frontfx, frontfy, frontcx, frontcy, frontk1, frontk2, frontk3, frontp1, frontp2);
					CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)	= tmppoint.x;
					CV_MAT_ELEM(*frontcoordinateyMat, float, y, x)	= tmppoint.y;
					CV_MAT_ELEM(*frontweightMat, float, y, x)		= D1/MaxBlend/2+0.5;	
					//后向
					CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rearcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rearweightMat, float, y, x)		= 0;
					//左向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertLeftStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertLeftBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, leftfx, leftfy, leftcx, leftcy, leftk1, leftk2, leftk3, leftp1, leftp2);
					CV_MAT_ELEM(*leftcoordinatexMat, float, y, x) = tmppoint.x;
					CV_MAT_ELEM(*leftcoordinateyMat, float, y, x) = tmppoint.y;
					CV_MAT_ELEM(*leftweightMat, float, y, x)		= 0.5-D1/MaxBlend/2;
					//右向
					CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rightweightMat, float, y, x)		= 0;
				}else if (D2 < MaxBlend)
				{
					//前向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertFrontStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertFrontBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, frontfx, frontfy, frontcx, frontcy, frontk1, frontk2, frontk3, frontp1, frontp2);
					CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)	= tmppoint.x;
					CV_MAT_ELEM(*frontcoordinateyMat, float, y, x)	= tmppoint.y;
					CV_MAT_ELEM(*frontweightMat, float, y, x)		= D2/MaxBlend/2+0.5;	
					//后向
					CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rearcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rearweightMat, float, y, x)		= 0;
					//左向
					CV_MAT_ELEM(*leftcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*leftcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*leftweightMat, float, y, x)		= 0;
					//右向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertRightStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertRightBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, rightfx, rightfy, rightcx, rightcy, rightk1, rightk2, rightk3, rightp1, rightp2);
					CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)	= tmppoint.x;
					CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)	= tmppoint.y;
					CV_MAT_ELEM(*rightweightMat, float, y, x)		= 0.5-D2/MaxBlend/2;
				}else
				{
					//前向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertFrontStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertFrontBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, frontfx, frontfy, frontcx, frontcy, frontk1, frontk2, frontk3, frontp1, frontp2);
					CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)	= tmppoint.x;
					CV_MAT_ELEM(*frontcoordinateyMat, float, y, x)	= tmppoint.y;
					CV_MAT_ELEM(*frontweightMat, float, y, x)		= 1;	
					//后向
					CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rearcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rearweightMat, float, y, x)		= 0;
					//左向
					CV_MAT_ELEM(*leftcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*leftcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*leftweightMat, float, y, x)		= 0;
					//右向
					CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rightweightMat, float, y, x)		= 0;
				}
			}else if (JudgeIsInside(cvPoint(x, y), rear->stitchPoint, 6))
			{
				A1 = rear->stitchPoint[4].y-rear->stitchPoint[3].y;
				B1 = rear->stitchPoint[3].x-rear->stitchPoint[4].x;
				C1 = rear->stitchPoint[4].x*rear->stitchPoint[3].y-rear->stitchPoint[4].x*rear->stitchPoint[3].y;
				AABB1 = sqrt(A1*A1+B1*B1);
				D1 = abs(A1*x+B1*y+C1)/AABB1;
				A2 = rear->stitchPoint[5].y-rear->stitchPoint[0].y;
				B2 = rear->stitchPoint[0].x-rear->stitchPoint[5].x;
				C2 = rear->stitchPoint[5].x*rear->stitchPoint[0].y-rear->stitchPoint[0].x*rear->stitchPoint[5].y;
				AABB2 = sqrt(A2*A2+B2*B2);
				D2 = abs(A2*x+B2*y+C2)/AABB2;
				if (D1 < MaxBlend)
				{
					//前向
					CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*frontcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*frontweightMat, float, y, x)		= 0;	
					//后向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertRearStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertRearBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, rearfx, rearfy, rearcx, rearcy, reark1, reark2, reark3, rearp1, rearp2);
					CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)	= tmppoint.x;
					CV_MAT_ELEM(*rearcoordinateyMat, float, y, x)	= tmppoint.y;
					CV_MAT_ELEM(*rearweightMat, float, y, x)		= D1/MaxBlend/2+0.5;
					//左向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertLeftStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertLeftBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, leftfx, leftfy, leftcx, leftcy, leftk1, leftk2, leftk3, leftp1, leftp2);
					CV_MAT_ELEM(*leftcoordinatexMat, float, y, x) = tmppoint.x;
					CV_MAT_ELEM(*leftcoordinateyMat, float, y, x) = tmppoint.y;
					CV_MAT_ELEM(*leftweightMat, float, y, x)		= 0.5-D1/MaxBlend/2;
					//右向
					CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rightweightMat, float, y, x)		= 0;
				}else if (D2 < MaxBlend)
				{
					//前向
					CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*frontcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*frontweightMat, float, y, x)		= 0;	
					//后向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertRearStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertRearBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, rearfx, rearfy, rearcx, rearcy, reark1, reark2, reark3, rearp1, rearp2);
					CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)	= tmppoint.x;
					CV_MAT_ELEM(*rearcoordinateyMat, float, y, x)	= tmppoint.y;
					CV_MAT_ELEM(*rearweightMat, float, y, x)		= D2/MaxBlend/2+0.5;
					//左向
					CV_MAT_ELEM(*leftcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*leftcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*leftweightMat, float, y, x)		= 0;
					//右向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertRightStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertRightBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, rightfx, rightfy, rightcx, rightcy, rightk1, rightk2, rightk3, rightp1, rightp2);
					CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)	= tmppoint.x;
					CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)	= tmppoint.y;
					CV_MAT_ELEM(*rightweightMat, float, y, x)		= 0.5-D2/MaxBlend/2;
				}else
				{
					//前向
					CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*frontcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*frontweightMat, float, y, x)		= 0;	
					//后向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertRearStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertRearBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, rearfx, rearfy, rearcx, rearcy, reark1, reark2, reark3, rearp1, rearp2);
					CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)	= tmppoint.x;
					CV_MAT_ELEM(*rearcoordinateyMat, float, y, x)	= tmppoint.y;
					CV_MAT_ELEM(*rearweightMat, float, y, x)		= 1;
					//左向
					CV_MAT_ELEM(*leftcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*leftcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*leftweightMat, float, y, x)		= 0;
					//右向
					CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rightweightMat, float, y, x)		= 0;
				}

			}else if (JudgeIsInside(cvPoint(x, y), left->stitchPoint, 6))
			{
				A1 = left->stitchPoint[4].y-left->stitchPoint[3].y;
				B1 = left->stitchPoint[3].x-left->stitchPoint[4].x;
				C1 = left->stitchPoint[4].x*left->stitchPoint[3].y-left->stitchPoint[3].x*left->stitchPoint[4].y;
				AABB1 = sqrt(A1*A1+B1*B1);
				D1 = abs(A1*x+B1*y+C1)/AABB1;
				A2 = left->stitchPoint[5].y-left->stitchPoint[0].y;
				B2 = left->stitchPoint[0].x-left->stitchPoint[5].x;
				C2 = left->stitchPoint[5].x*left->stitchPoint[0].y-left->stitchPoint[0].x*left->stitchPoint[5].y;
				AABB2 = sqrt(A2*A2+B2*B2);
				D2 = abs(A2*x+B2*y+C2)/AABB2;

				if (D1 < MaxBlend)
				{
					//前向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertFrontStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertFrontBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, frontfx, frontfy, frontcx, frontcy, frontk1, frontk2, frontk3, frontp1, frontp2);
					CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)	= tmppoint.x;
					CV_MAT_ELEM(*frontcoordinateyMat, float, y, x)	= tmppoint.y;
					CV_MAT_ELEM(*frontweightMat, float, y, x)		= 0.5-D1/MaxBlend/2;
					//后向
					CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rearcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rearweightMat, float, y, x)		= 0;
					//左向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertLeftStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertLeftBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, leftfx, leftfy, leftcx, leftcy, leftk1, leftk2, leftk3, leftp1, leftp2);
					CV_MAT_ELEM(*leftcoordinatexMat, float, y, x) = tmppoint.x;
					CV_MAT_ELEM(*leftcoordinateyMat, float, y, x) = tmppoint.y;
					CV_MAT_ELEM(*leftweightMat, float, y, x) = D1/MaxBlend/2+0.5;
					//右向
					CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rightweightMat, float, y, x)		= 0;
				}else if (D2 < MaxBlend)
				{
					//前向
					CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*frontcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*frontweightMat, float, y, x)		= 0;
					//后向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertRearStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertRearBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, rearfx, rearfy, rearcx, rearcy, reark1, reark2, reark3, rearp1, rearp2);
					CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)	= tmppoint.x;
					CV_MAT_ELEM(*rearcoordinateyMat, float, y, x)	= tmppoint.y;
					CV_MAT_ELEM(*rearweightMat, float, y, x)		= 0.5-D2/MaxBlend/2;
					//左向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertLeftStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertLeftBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, leftfx, leftfy, leftcx, leftcy, leftk1, leftk2, leftk3, leftp1, leftp2);
					CV_MAT_ELEM(*leftcoordinatexMat, float, y, x) = tmppoint.x;
					CV_MAT_ELEM(*leftcoordinateyMat, float, y, x) = tmppoint.y;
					CV_MAT_ELEM(*leftweightMat, float, y, x) = D2/MaxBlend/2+0.5;
					//右向
					CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rightweightMat, float, y, x)		= 0;
				}else
				{
					//前向
					CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*frontcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*frontweightMat, float, y, x)		= 0;
					//后向
					CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rearcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rearweightMat, float, y, x)		= 0;
					//左向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertLeftStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertLeftBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, leftfx, leftfy, leftcx, leftcy, leftk1, leftk2, leftk3, leftp1, leftp2);
					CV_MAT_ELEM(*leftcoordinatexMat, float, y, x) = tmppoint.x;
					CV_MAT_ELEM(*leftcoordinateyMat, float, y, x) = tmppoint.y;
					CV_MAT_ELEM(*leftweightMat, float, y, x) = 1;
					//右向
					CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rightweightMat, float, y, x)		= 0;
				}
			}else if (JudgeIsInside(cvPoint(x, y), right->stitchPoint, 6))
			{
				A1 = right->stitchPoint[5].y-right->stitchPoint[0].y;
				B1 = right->stitchPoint[0].x-right->stitchPoint[5].x;
				C1 = right->stitchPoint[5].x*right->stitchPoint[0].y-right->stitchPoint[0].x*right->stitchPoint[5].y;
				AABB1 = sqrt(A1*A1+B1*B1);
				D1 = abs(A1*x+B1*y+C1)/AABB1;
				A2 = right->stitchPoint[4].y-right->stitchPoint[3].y;
				B2 = right->stitchPoint[3].x-right->stitchPoint[4].x;
				C2 = right->stitchPoint[4].x*right->stitchPoint[3].y-right->stitchPoint[3].x*right->stitchPoint[4].y;
				AABB2 = sqrt(A2*A2+B2*B2);
				D2 = abs(A2*x+B2*y+C2)/AABB2;

				if (D1 < MaxBlend)
				{
					//前向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertFrontStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertFrontBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, frontfx, frontfy, frontcx, frontcy, frontk1, frontk2, frontk3, frontp1, frontp2);
					CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)	= tmppoint.x;
					CV_MAT_ELEM(*frontcoordinateyMat, float, y, x)	= tmppoint.y;
					CV_MAT_ELEM(*frontweightMat, float, y, x)		= 0.5-D1/MaxBlend/2;
					//后向
					CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rearcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rearweightMat, float, y, x)		= 0;
					//左向
					CV_MAT_ELEM(*leftcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*leftcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*leftweightMat, float, y, x)		= 0;
					//右向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertRightStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertRightBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, rightfx, rightfy, rightcx, rightcy, rightk1, rightk2, rightk3, rightp1, rightp2);
					CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)	= tmppoint.x;
					CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)	= tmppoint.y;
					CV_MAT_ELEM(*rightweightMat, float, y, x)		= D1/MaxBlend/2+0.5;
				}else if (D2 < MaxBlend)
				{
					//前向
					CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*frontcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*frontweightMat, float, y, x)		= 0;
					//后向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertRearStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertRearBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, rearfx, rearfy, rearcx, rearcy, reark1, reark2, reark3, rearp1, rearp2);
					CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)	= tmppoint.x;
					CV_MAT_ELEM(*rearcoordinateyMat, float, y, x)	= tmppoint.y;
					CV_MAT_ELEM(*rearweightMat, float, y, x)		= 0.5-D2/MaxBlend/2;
					//左向
					CV_MAT_ELEM(*leftcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*leftcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*leftweightMat, float, y, x)		= 0;
					//右向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertRightStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertRightBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, rightfx, rightfy, rightcx, rightcy, rightk1, rightk2, rightk3, rightp1, rightp2);
					CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)	= tmppoint.x;
					CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)	= tmppoint.y;
					CV_MAT_ELEM(*rightweightMat, float, y, x)		= D2/MaxBlend/2+0.5;
				}else
				{
					//前向
					CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*frontcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*frontweightMat, float, y, x)		= 0;
					//后向
					CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rearcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*rearweightMat, float, y, x)		= 0;
					//左向
					CV_MAT_ELEM(*leftcoordinatexMat, float, y, x)	= 0;
					CV_MAT_ELEM(*leftcoordinateyMat, float, y, x)	= 0;
					CV_MAT_ELEM(*leftweightMat, float, y, x)		= 0;
					//右向
					tmppoint = cvPoint2D32f(x, y);
					TransformPoint(&tmppoint, 1, invertRightStitchMat, &tmppoint);
					TransformPoint(&tmppoint, 1, invertRightBirdMat, &tmppoint);
					GetDistorPoint(&tmppoint, 1, &tmppoint, rightfx, rightfy, rightcx, rightcy, rightk1, rightk2, rightk3, rightp1, rightp2);
					CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)	= tmppoint.x;
					CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)	= tmppoint.y;
					CV_MAT_ELEM(*rightweightMat, float, y, x)		= 1;
				}
			}else
			{
				//前向
				CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*frontcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*frontweightMat, float, y, x)		= 0;
				//后向
				CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rearcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rearweightMat, float, y, x)	= 0;
				//左向
				CV_MAT_ELEM(*leftcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*leftcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*leftweightMat, float, y, x)		= 0;
				//右向
				CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)	= 0;
				CV_MAT_ELEM(*rightweightMat, float, y, x)		= 0;
			}
		}
	}
	printf("stop writting map\n");
	cvSave(FRONT_COORDINATE_X_FILE_NAME, frontcoordinatexMat);
	cvSave(FRONT_COORDINATE_Y_FILE_NAME, frontcoordinateyMat);
	cvSave(FRONT_WEIGHT_FILE_NAME, frontweightMat);
	cvSave(REAR_COORDINATE_X_FILE_NAME, rearcoordinatexMat);
	cvSave(REAR_COORDINATE_Y_FILE_NAME, rearcoordinateyMat);
	cvSave(REAR_WEIGHT_FILE_NAME, rearweightMat);
	cvSave(LEFT_COORDINATE_X_FILE_NAME, leftcoordinatexMat);
	cvSave(LEFT_COORDINATE_Y_FILE_NAME, leftcoordinateyMat);
	cvSave(LEFT_WEIGHT_FILE_NAME, leftweightMat);
	cvSave(RIGHT_COORDINATE_X_FILE_NAME, rightcoordinatexMat);
	cvSave(RIGHT_COORDINATE_Y_FILE_NAME, rightcoordinateyMat);
	cvSave(RIGHT_WEIGHT_FILE_NAME, rightweightMat);

	cvReleaseMat(&frontcoordinatexMat);
	cvReleaseMat(&frontcoordinateyMat);
	cvReleaseMat(&frontweightMat);
	cvReleaseMat(&rearcoordinatexMat);
	cvReleaseMat(&rearcoordinateyMat);
	cvReleaseMat(&rearweightMat);
	cvReleaseMat(&leftcoordinatexMat);
	cvReleaseMat(&leftcoordinateyMat);
	cvReleaseMat(&leftweightMat);
	cvReleaseMat(&rightcoordinatexMat);
	cvReleaseMat(&rightcoordinateyMat);
	cvReleaseMat(&rightweightMat);
	cvReleaseMat(&invertFrontBirdMat);
	cvReleaseMat(&invertFrontStitchMat);
	cvReleaseMat(&invertRearBirdMat);
	cvReleaseMat(&invertRearStitchMat);
	cvReleaseMat(&invertLeftBirdMat);
	cvReleaseMat(&invertLeftStitchMat);
	cvReleaseMat(&invertRightBirdMat);
	cvReleaseMat(&invertRightStitchMat);
	return true;
}
/************************************************************************/
/*				释放拼接参数内存		                                */
/************************************************************************/
bool ReleaseParam(StitchInfo *front, StitchInfo *rear, StitchInfo *left, StitchInfo *right)
{
	if (!front || !rear || !left || !right)
	{
		return false;
	}
	//释入图像内存
	cvReleaseImage(&front->srcImage);
	cvReleaseImage(&front->undistorImage);
	cvReleaseImage(&front->birdImage);
	cvReleaseImage(&front->transformImage);
	cvReleaseImage(&front->stitchImage);
	cvReleaseImage(&rear->srcImage);
	cvReleaseImage(&rear->undistorImage);
	cvReleaseImage(&rear->birdImage);
	cvReleaseImage(&rear->transformImage);
	cvReleaseImage(&left->srcImage);
	cvReleaseImage(&left->undistorImage);
	cvReleaseImage(&left->birdImage);
	cvReleaseImage(&left->transformImage);
	cvReleaseImage(&right->srcImage);
	cvReleaseImage(&right->undistorImage);
	cvReleaseImage(&right->birdImage);
	cvReleaseImage(&right->transformImage);
	//释放畸变校正表内存
	cvReleaseImage(&front->mapx);
	cvReleaseImage(&front->mapy);
	cvReleaseImage(&rear->mapx);
	cvReleaseImage(&rear->mapy);
	cvReleaseImage(&left->mapx);
	cvReleaseImage(&left->mapy);
	cvReleaseImage(&right->mapx);
	cvReleaseImage(&right->mapy);
	//释放点
	delete[] front->srcCorners;
	delete[] front->birdCorners;
	delete[] front->srcMarkPoint;
	delete[] front->birdMarkPoint;
	delete[] rear->srcCorners;
	delete[] rear->birdCorners;
	delete[] rear->srcMarkPoint;
	delete[] rear->birdMarkPoint;
	delete[] left->srcCorners;
	delete[] left->birdCorners;
	delete[] left->srcMarkPoint;
	delete[] left->birdMarkPoint;
	delete[] right->srcCorners;
	delete[] right->birdCorners;
	delete[] right->srcMarkPoint;
	delete[] right->birdMarkPoint;
	delete[] front->stitchPoint;
	delete[] rear->stitchPoint;
	delete[] left->stitchPoint;
	delete[] right->stitchPoint;
	delete[] front->stitchMarkPoint;
	delete[] rear->stitchMarkPoint;
	delete[] left->stitchMarkPoint;
	delete[] right->stitchMarkPoint;
	//释放转换矩阵
	cvReleaseMat(&front->birdMat);
	cvReleaseMat(&front->stitchMat);
	cvReleaseMat(&rear->birdMat);
	cvReleaseMat(&rear->stitchMat);
	cvReleaseMat(&left->birdMat);
	cvReleaseMat(&left->stitchMat);
	cvReleaseMat(&right->birdMat);
	cvReleaseMat(&right->stitchMat);

	return true;
}
/************************************************************************/
/*				初始化拼接表                                            */
/*              根据全景表生成全景图                                    */
/*				float *frontcoordinatexMat: 前向X坐标表					*/
/*				float *frontcoordinateyMat: 前向Y坐标表					*/
/*				float *frontweightMat:		前向权值表					*/
/*				float *rearcoordinatexMat:	后向X坐标表					*/
/*				float *rearcoordinateyMat:	后向Y坐标表					*/
/*				float *rearweightMat:		后向权值表					*/
/*				float *leftcoordinatexMat:	左向X坐标表					*/
/*				float *leftcoordinateyMat:	左向Y坐标表					*/
/*				float *leftweightMat:		左向权值表					*/
/*				float *rightcoordinatexMat: 右向X坐标表					*/
/*				float *rightcoordinateyMat: 右向Y坐标表					*/
/************************************************************************/
bool InitStitchMap(float **frontcoordinatex, float **frontcoordinatey, float **frontweight, float **rearcoordinatex, float **rearcoordinatey, float **rearweight,
	float **leftcoordinatex, float **leftcoordinatey, float **leftweight, float **rightcoordinatex, float **rightcoordinatey, float **rightweight)
{
	CvMat *frontcoordinatexMat		= (CvMat*)cvLoad(FRONT_COORDINATE_X_FILE_NAME);
	CvMat *frontcoordinateyMat		= (CvMat*)cvLoad(FRONT_COORDINATE_Y_FILE_NAME);
	CvMat *frontweightMat			= (CvMat*)cvLoad(FRONT_WEIGHT_FILE_NAME);
	CvMat *rearcoordinatexMat		= (CvMat*)cvLoad(REAR_COORDINATE_X_FILE_NAME);
	CvMat *rearcoordinateyMat		= (CvMat*)cvLoad(REAR_COORDINATE_Y_FILE_NAME);
	CvMat *rearweightMat			= (CvMat*)cvLoad(REAR_WEIGHT_FILE_NAME);
	CvMat *leftcoordinatexMat		= (CvMat*)cvLoad(LEFT_COORDINATE_X_FILE_NAME);
	CvMat *leftcoordinateyMat		= (CvMat*)cvLoad(LEFT_COORDINATE_Y_FILE_NAME);
	CvMat *leftweightMat			= (CvMat*)cvLoad(LEFT_WEIGHT_FILE_NAME);
	CvMat *rightcoordinatexMat		= (CvMat*)cvLoad(RIGHT_COORDINATE_X_FILE_NAME);
	CvMat *rightcoordinateyMat		= (CvMat*)cvLoad(RIGHT_COORDINATE_Y_FILE_NAME);
	CvMat *rightweightMat			= (CvMat*)cvLoad(RIGHT_WEIGHT_FILE_NAME);

	if (!frontcoordinatexMat || !frontcoordinateyMat || !frontweightMat || !rearcoordinatexMat || !rearcoordinateyMat || !rearweightMat || 
		!leftcoordinatexMat || !leftcoordinateyMat || !leftweightMat || !rightcoordinatexMat || !rightcoordinateyMat || !rightweightMat)
	{
		printf("读取拼接表失败\n");
		return false;
	}
	*frontcoordinatex	= new float[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*frontcoordinatey	= new float[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*frontweight			= new float[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rearcoordinatex		= new float[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rearcoordinatey		= new float[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rearweight			= new float[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*leftcoordinatex		= new float[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*leftcoordinatey		= new float[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*leftweight			= new float[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rightcoordinatex	= new float[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rightcoordinatey	= new float[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rightweight			= new float[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];

	for (int y = 0; y < STITCH_IMAGE_HEIGHT; ++y)
	{
		for (int x = 0; x < STITCH_IMAGE_WIDTH; ++x)
		{
			*(*frontcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= CV_MAT_ELEM(*frontcoordinatexMat, float, y, x);
			*(*frontcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= CV_MAT_ELEM(*frontcoordinateyMat, float, y, x);
			*(*frontweight+y*STITCH_IMAGE_WIDTH+x)		= CV_MAT_ELEM(*frontweightMat, float, y, x);
			*(*rearcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= CV_MAT_ELEM(*rearcoordinatexMat, float, y, x);
			*(*rearcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= CV_MAT_ELEM(*rearcoordinateyMat, float, y, x);
			*(*rearweight+y*STITCH_IMAGE_WIDTH+x)		= CV_MAT_ELEM(*rearweightMat, float, y, x);
			*(*leftcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= CV_MAT_ELEM(*leftcoordinatexMat, float, y, x);
			*(*leftcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= CV_MAT_ELEM(*leftcoordinateyMat, float, y, x);
			*(*leftweight+y*STITCH_IMAGE_WIDTH+x)		= CV_MAT_ELEM(*leftweightMat, float, y, x);
			*(*rightcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= CV_MAT_ELEM(*rightcoordinatexMat, float, y, x);
			*(*rightcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= CV_MAT_ELEM(*rightcoordinateyMat, float, y, x);
			*(*rightweight+y*STITCH_IMAGE_WIDTH+x)		= CV_MAT_ELEM(*rightweightMat, float, y, x);
		}
	}

	cvReleaseMat(&frontcoordinatexMat);
	cvReleaseMat(&frontcoordinateyMat);
	cvReleaseMat(&frontweightMat);
	cvReleaseMat(&rearcoordinatexMat);
	cvReleaseMat(&rearcoordinateyMat);
	cvReleaseMat(&rearweightMat);
	cvReleaseMat(&leftcoordinatexMat);
	cvReleaseMat(&leftcoordinateyMat);
	cvReleaseMat(&leftweightMat);
	cvReleaseMat(&rightcoordinatexMat);
	cvReleaseMat(&rightcoordinateyMat);
	cvReleaseMat(&rightweightMat);
	return true;
}
/************************************************************************/
/*				单线程调用												*/
/*              根据全景表生成全景图                                    */
/*				float *frontcoordinatexMat: 前向X坐标表					*/
/*				float *frontcoordinateyMat: 前向Y坐标表					*/
/*				float *frontweightMat:		前向权值表					*/
/*				float *rearcoordinatexMat:	后向X坐标表					*/
/*				float *rearcoordinateyMat:	后向Y坐标表					*/
/*				float *rearweightMat:		后向权值表					*/
/*				float *leftcoordinatexMat:	左向X坐标表					*/
/*				float *leftcoordinateyMat:	左向Y坐标表					*/
/*				float *leftweightMat:		左向权值表					*/
/*				float *rightcoordinatexMat: 右向X坐标表					*/
/*				float *rightcoordinateyMat: 右向Y坐标表					*/
/*				float *rightweightMat:		右向权值表					*/
/*				unsigned char *frontImage	前向源图像内存地址			*/
/*				unsigned char *rearImage	后向源图像内存地址			*/
/*				unsigned char *leftImage	左向源图像内存地址			*/
/*				unsigned char *rightImage	右向源图像内存地址			*/
/*				unsigned char *stitchImage	输出全景图像内存地址		*/
/************************************************************************/
bool GetStitchImage(float *frontcoordinatexMat, float *frontcoordinateyMat, float *frontweightMat, float *rearcoordinatexMat, float *rearcoordinateyMat,
	float *rearweightMat, float *leftcoordinatexMat, float *leftcoordinateyMat, float *leftweightMat, float *rightcoordinatexMat, float *rightcoordinateyMat, 
	float *rightweightMat, unsigned char *frontImage,  unsigned char *rearImage,  unsigned char *leftImage,  unsigned char *rightImage,  
	unsigned char *stitchImage)
{
	if (!frontImage || !rearImage || !leftImage || !rightImage || !stitchImage)
	{
		return false;
	}

	if (!frontcoordinatexMat || !frontcoordinateyMat || !frontweightMat || !rearcoordinatexMat || !rearcoordinateyMat || !rearweightMat || 
		!leftcoordinatexMat || !leftcoordinateyMat || !leftweightMat || !rightcoordinatexMat || !rightcoordinateyMat || !rightweightMat)
	{
		return false;
	}
	float		frontweight, rearweight, leftweight, rightweight;
	float		frontR, frontG, frontB, rearR, rearG, rearB, leftR, leftG, leftB, rightR, rightG, rightB;
	float		tmpvalR1, tmpvalR2, tmpvalR3, tmpvalR4, dstvalR, dstvalG, dstvalB;
	float		tmpvalG1, tmpvalG2, tmpvalG3, tmpvalG4;
	float		tmpvalB1, tmpvalB2, tmpvalB3, tmpvalB4;
	float		srcx, srcy, px, py;
	for (int x = 0; x < STITCH_IMAGE_WIDTH; ++x)
	{
		for (int y = 0; y < STITCH_IMAGE_HEIGHT; ++y)
		{
			frontweight = *frontweightMat+y*STITCH_IMAGE_WIDTH+x;
			rearweight	= *rearweightMat+y*STITCH_IMAGE_WIDTH+x;
			leftweight	= *leftweightMat+y*STITCH_IMAGE_WIDTH+x;
			rightweight = *rightweightMat+y*STITCH_IMAGE_WIDTH+x;
			//front
			if (frontweight < 0.001)
			{
				frontR = 0;
				frontG = 0;
				frontB = 0;
			}else
			{
				srcx = *(frontcoordinatexMat+y*STITCH_IMAGE_WIDTH+x);
				srcy = *(frontcoordinateyMat+y*STITCH_IMAGE_WIDTH+x);
				if ((int)(srcx) >= 0 && (int)(srcx+1) < IMAGE_WIDTH &&
					(int)(srcy) >= 0 && (int)(srcy+1) < IMAGE_HEIGHT)
				{
					tmpvalB1 = *(frontImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4);
					tmpvalB2 = *(frontImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4);
					tmpvalB3 = *(frontImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4);
					tmpvalB4 = *(frontImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4);
					tmpvalG1 = *(frontImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+1);
					tmpvalG2 = *(frontImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+1);
					tmpvalG3 = *(frontImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+1);
					tmpvalG4 = *(frontImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+1);
					tmpvalR1 = *(frontImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+2);
					tmpvalR2 = *(frontImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+2);
					tmpvalR3 = *(frontImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+2);
					tmpvalR4 = *(frontImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+2);
					px = srcx-(int)(srcx);
					py = srcy-(int)(srcy);
					frontR = (1-px)*(1-py)*tmpvalR1 + px*(1-py)*tmpvalR2 + (1-px)*py*tmpvalR3 + px*py*tmpvalR4;
					frontG = (1-px)*(1-py)*tmpvalG1 + px*(1-py)*tmpvalG2 + (1-px)*py*tmpvalG3 + px*py*tmpvalG4;
					frontB = (1-px)*(1-py)*tmpvalB1 + px*(1-py)*tmpvalB2 + (1-px)*py*tmpvalB3 + px*py*tmpvalB4;
				}else
				{
					frontR = 0;
					frontG = 0;
					frontB = 0;
				}

			}
			//rear
			if (rearweight < 0.001)
			{
				rearR = 0;
				rearG = 0;
				rearB = 0;
			}else
			{
				srcx = *(rearcoordinatexMat+y*STITCH_IMAGE_WIDTH+x);
				srcy = *(rearcoordinateyMat+y*STITCH_IMAGE_WIDTH+x);
				if ((int)(srcx) >= 0 && (int)(srcx+1) < IMAGE_WIDTH &&
					(int)(srcy) >= 0 && (int)(srcy+1) < IMAGE_HEIGHT)
				{
					tmpvalB1 = *(rearImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4);
					tmpvalB2 = *(rearImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4);
					tmpvalB3 = *(rearImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4);
					tmpvalB4 = *(rearImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4);
					tmpvalG1 = *(rearImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+1);
					tmpvalG2 = *(rearImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+1);
					tmpvalG3 = *(rearImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+1);
					tmpvalG4 = *(rearImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+1);
					tmpvalR1 = *(rearImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+2);
					tmpvalR2 = *(rearImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+2);
					tmpvalR3 = *(rearImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+2);
					tmpvalR4 = *(rearImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+2);
					px = srcx-(int)(srcx);
					py = srcy-(int)(srcy);
					rearR = (1-px)*(1-py)*tmpvalR1 + px*(1-py)*tmpvalR2 + (1-px)*py*tmpvalR3 + px*py*tmpvalR4;
					rearG = (1-px)*(1-py)*tmpvalG1 + px*(1-py)*tmpvalG2 + (1-px)*py*tmpvalG3 + px*py*tmpvalG4;
					rearB = (1-px)*(1-py)*tmpvalB1 + px*(1-py)*tmpvalB2 + (1-px)*py*tmpvalB3 + px*py*tmpvalB4;
				}else
				{
					rearR = 0;
					rearG = 0;
					rearB = 0;
				}

			}
			//left
			if (leftweight < 0.001)
			{
				leftR = 0;
				leftG = 0;
				leftB = 0;
			}else
			{
				srcx = *(leftcoordinatexMat+y*STITCH_IMAGE_WIDTH+x);
				srcy = *(leftcoordinateyMat+y*STITCH_IMAGE_WIDTH+x);
				if ((int)(srcx) >= 0 && (int)(srcx+1) < IMAGE_WIDTH &&
					(int)(srcy) >= 0 && (int)(srcy+1) < IMAGE_HEIGHT)
				{
					tmpvalB1 = *(leftImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4);
					tmpvalB2 = *(leftImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4);
					tmpvalB3 = *(leftImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4);
					tmpvalB4 = *(leftImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4);
					tmpvalG1 = *(leftImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+1);
					tmpvalG2 = *(leftImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+1);
					tmpvalG3 = *(leftImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+1);
					tmpvalG4 = *(leftImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+1);
					tmpvalR1 = *(leftImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+2);
					tmpvalR2 = *(leftImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+2);
					tmpvalR3 = *(leftImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+2);
					tmpvalR4 = *(leftImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+2);
					px = srcx-(int)(srcx);
					py = srcy-(int)(srcy);
					leftR = (1-px)*(1-py)*tmpvalR1 + px*(1-py)*tmpvalR2 + (1-px)*py*tmpvalR3 + px*py*tmpvalR4;
					leftG = (1-px)*(1-py)*tmpvalG1 + px*(1-py)*tmpvalG2 + (1-px)*py*tmpvalG3 + px*py*tmpvalG4;
					leftB = (1-px)*(1-py)*tmpvalB1 + px*(1-py)*tmpvalB2 + (1-px)*py*tmpvalB3 + px*py*tmpvalB4;
				}else
				{
					leftR = 0;
					leftG = 0;
					leftB = 0;
				}

			}
			//right
			if (rightweight < 0.001)
			{
				rightR = 0;
				rightG = 0;
				rightB = 0;
			}else
			{
				srcx = *(rightcoordinatexMat+y*STITCH_IMAGE_WIDTH+x);
				srcy = *(rightcoordinateyMat+y*STITCH_IMAGE_WIDTH+x);
				if ((int)(srcx) >= 0 && (int)(srcx+1) < IMAGE_WIDTH &&
					(int)(srcy) >= 0 && (int)(srcy+1) < IMAGE_HEIGHT)
				{
					tmpvalB1 = *(rightImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4);
					tmpvalB2 = *(rightImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4);
					tmpvalB3 = *(rightImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4);
					tmpvalB4 = *(rightImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4);
					tmpvalG1 = *(rightImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+1);
					tmpvalG2 = *(rightImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+1);
					tmpvalG3 = *(rightImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+1);
					tmpvalG4 = *(rightImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+1);
					tmpvalR1 = *(rightImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+2);
					tmpvalR2 = *(rightImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+2);
					tmpvalR3 = *(rightImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+2);
					tmpvalR4 = *(rightImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+2);
					px = srcx-(int)(srcx);
					py = srcy-(int)(srcy);
					rightR = (1-px)*(1-py)*tmpvalR1 + px*(1-py)*tmpvalR2 + (1-px)*py*tmpvalR3 + px*py*tmpvalR4;
					rightG = (1-px)*(1-py)*tmpvalG1 + px*(1-py)*tmpvalG2 + (1-px)*py*tmpvalG3 + px*py*tmpvalG4;
					rightB = (1-px)*(1-py)*tmpvalB1 + px*(1-py)*tmpvalB2 + (1-px)*py*tmpvalB3 + px*py*tmpvalB4;
				}else
				{
					rightR = 0;
					rightG = 0;
					rightB = 0;
				}
			}
			//dstval
			dstvalB = frontweight*frontB+rearweight*rearB+leftweight*leftB+rightweight*rightB;
			dstvalG = frontweight*frontG+rearweight*rearG+leftweight*leftG+rightweight*rightG;
			dstvalR = frontweight*frontR+rearweight*rearR+leftweight*leftR+rightweight*rightR;
			if (dstvalB > 255)
			{
				dstvalB = 255;
			}
			if (dstvalG > 255)
			{
				dstvalG = 255;
			}
			if (dstvalR > 255)
			{
				dstvalR = 255;
			}
			//*(stitchImage+(y*STITCH_IMAGE_WIDTH+x)*4)	= dstvalB;
			//*(stitchImage+(y*STITCH_IMAGE_WIDTH+x)*4+1) = dstvalG;
			//*(stitchImage+(y*STITCH_IMAGE_WIDTH+x)*4+2) = dstvalR;
			//*(stitchImage+(y*STITCH_IMAGE_WIDTH+x)*4+3) = 0;
			*(stitchImage+(y*STITCH_IMAGE_WIDTH+x)*4)	= frontweight*frontB+rearweight*rearB+leftweight*leftB+rightweight*rightB;
			*(stitchImage+(y*STITCH_IMAGE_WIDTH+x)*4+1) = frontweight*frontG+rearweight*rearG+leftweight*leftG+rightweight*rightG;
			*(stitchImage+(y*STITCH_IMAGE_WIDTH+x)*4+2) = frontweight*frontR+rearweight*rearR+leftweight*leftR+rightweight*rightR;
			*(stitchImage+(y*STITCH_IMAGE_WIDTH+x)*4+3) = 0;
		}
	}

	return true;
}
/************************************************************************/
/*				释放拼接表		                                        */
/*				unsigned int *frontcoordinatexMat: 前向X坐标表			*/
/*				unsigned int *frontcoordinateyMat: 前向Y坐标表			*/
/*				unsigned int *frontweightMat:		前向权值表			*/
/*				unsigned int *rearcoordinatexMat:	后向X坐标表			*/
/*				unsigned int *rearcoordinateyMat:	后向Y坐标表			*/
/*				unsigned int *rearweightMat:		后向权值表			*/
/*				unsigned int *leftcoordinatexMat:	左向X坐标表			*/
/*				unsigned int *leftcoordinateyMat:	左向Y坐标表			*/
/*				unsigned int *leftweightMat:		左向权值表			*/
/*				unsigned int *rightcoordinatexMat: 右向X坐标表			*/
/*				unsigned int *rightcoordinateyMat: 右向Y坐标表			*/
/*				unsigned int *rightweightMat:		右向权值表			*/
/************************************************************************/
bool ReleaseStitchMap(unsigned int **frontcoordinatexMat, unsigned int **frontcoordinateyMat, unsigned int **frontweightMat, unsigned int **rearcoordinatexMat, 
	unsigned int **rearcoordinateyMat, unsigned int **rearweightMat, unsigned int **leftcoordinatexMat, unsigned int **leftcoordinateyMat, 
	unsigned int **leftweightMat, unsigned int **rightcoordinatexMat, unsigned int **rightcoordinateyMat, unsigned int **rightweightMat)
{
	if (!frontcoordinatexMat || !frontcoordinateyMat || !frontweightMat || !rearcoordinatexMat || !rearcoordinateyMat || !rearweightMat || 
		!leftcoordinatexMat || !leftcoordinateyMat || !leftweightMat || !rightcoordinatexMat || !rightcoordinateyMat || !rightweightMat)
	{
		return false;
	}
	delete[] *frontcoordinatexMat;
	delete[] *frontcoordinateyMat;
	delete[] *frontweightMat;
	delete[] *rearcoordinatexMat;
	delete[] *rearcoordinateyMat;
	delete[] *rearweightMat;
	delete[] *leftcoordinatexMat;
	delete[] *leftcoordinateyMat;
	delete[] *leftweightMat;
	delete[] *rightcoordinatexMat;
	delete[] *rightcoordinateyMat;
	delete[] *rightweightMat;
	return true;
}
/************************************************************************/
/*				多线程调用												*/
/*              根据全景表生成全景图                                    */
/*				float *frontcoordinatexMat: 前向X坐标表					*/
/*				float *frontcoordinateyMat: 前向Y坐标表					*/
/*				float *frontweightMat:		前向权值表					*/
/*				float *rearcoordinatexMat:	后向X坐标表					*/
/*				float *rearcoordinateyMat:	后向Y坐标表					*/
/*				float *rearweightMat:		后向权值表					*/
/*				float *leftcoordinatexMat:	左向X坐标表					*/
/*				float *leftcoordinateyMat:	左向Y坐标表					*/
/*				float *leftweightMat:		左向权值表					*/
/*				float *rightcoordinatexMat: 右向X坐标表					*/
/*				float *rightcoordinateyMat: 右向Y坐标表					*/
/*				float *rightweightMat:		右向权值表					*/
/*				unsigned char *frontImage	前向源图像内存地址			*/
/*				unsigned char *rearImage	后向源图像内存地址			*/
/*				unsigned char *leftImage	左向源图像内存地址			*/
/*				unsigned char *rightImage	右向源图像内存地址			*/
/*				unsigned char *stitchImage	输出全景图像内存地址		*/
/*				int xoffset:				x坐标偏移量					*/
/*				int yoffset:				y坐标偏移量					*/
/*				int dstwidthstep:			显示到显示屏的宽			*/
/*				int NX:						水平方向被分成NX部分		*/
/*				int NY:						垂直方向被分成NY部分		*/
/************************************************************************/
bool GetStitchImage2(float *frontcoordinatexMat, float *frontcoordinateyMat, float *frontweightMat, float *rearcoordinatexMat, float *rearcoordinateyMat,
	float *rearweightMat, float *leftcoordinatexMat, float *leftcoordinateyMat, float *leftweightMat, float *rightcoordinatexMat, float *rightcoordinateyMat, 
	float *rightweightMat, unsigned char *frontImage,  unsigned char *rearImage,  unsigned char *leftImage,  unsigned char *rightImage,  
	unsigned char *stitchImage, int xoffset, int yoffset, int dstwidthstep, int NX, int NY)
{
	if (!frontImage || !rearImage || !leftImage || !rightImage || !stitchImage)
	{
		return false;
	}

	if (!frontcoordinatexMat || !frontcoordinateyMat || !frontweightMat || !rearcoordinatexMat || !rearcoordinateyMat || !rearweightMat || 
		!leftcoordinatexMat || !leftcoordinateyMat || !leftweightMat || !rightcoordinatexMat || !rightcoordinateyMat || !rightweightMat)
	{
		return false;
	}
	float		frontweight, rearweight, leftweight, rightweight;
	float		frontR, frontG, frontB, rearR, rearG, rearB, leftR, leftG, leftB, rightR, rightG, rightB;
	float		tmpvalR1, tmpvalR2, tmpvalR3, tmpvalR4, dstvalR, dstvalG, dstvalB;
	float		tmpvalG1, tmpvalG2, tmpvalG3, tmpvalG4;
	float		tmpvalB1, tmpvalB2, tmpvalB3, tmpvalB4;
	float		srcx, srcy, px, py;
	unsigned char *dst; 
	for (int y = yoffset; y < yoffset+STITCH_IMAGE_HEIGHT/NY; ++y)
	{
		dst = stitchImage+(y*dstwidthstep+xoffset)*4;
		for (int x = xoffset; x < xoffset+STITCH_IMAGE_WIDTH/NX; ++x)
		{
			frontweight = *(frontweightMat+y*STITCH_IMAGE_WIDTH+x);
			rearweight	= *(rearweightMat+y*STITCH_IMAGE_WIDTH+x);
			leftweight	= *(leftweightMat+y*STITCH_IMAGE_WIDTH+x);
			rightweight = *(rightweightMat+y*STITCH_IMAGE_WIDTH+x);
			//front
			if (frontweight < 0.001)
			{
				frontR = 0;
				frontG = 0;
				frontB = 0;
			}else
			{
				srcx = *(frontcoordinatexMat+y*STITCH_IMAGE_WIDTH+x);
				srcy = *(frontcoordinateyMat+y*STITCH_IMAGE_WIDTH+x);
				if ((int)(srcx) >= 0 && (int)(srcx+1) < IMAGE_WIDTH &&
					(int)(srcy) >= 0 && (int)(srcy+1) < IMAGE_HEIGHT)
				{
					tmpvalB1 = *(frontImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4);
					tmpvalB2 = *(frontImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4);
					tmpvalB3 = *(frontImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4);
					tmpvalB4 = *(frontImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4);
					tmpvalG1 = *(frontImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+1);
					tmpvalG2 = *(frontImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+1);
					tmpvalG3 = *(frontImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+1);
					tmpvalG4 = *(frontImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+1);
					tmpvalR1 = *(frontImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+2);
					tmpvalR2 = *(frontImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+2);
					tmpvalR3 = *(frontImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+2);
					tmpvalR4 = *(frontImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+2);
					px = srcx-(int)(srcx);
					py = srcy-(int)(srcy);
					frontR = (1-px)*(1-py)*tmpvalR1 + px*(1-py)*tmpvalR2 + (1-px)*py*tmpvalR3 + px*py*tmpvalR4;
					frontG = (1-px)*(1-py)*tmpvalG1 + px*(1-py)*tmpvalG2 + (1-px)*py*tmpvalG3 + px*py*tmpvalG4;
					frontB = (1-px)*(1-py)*tmpvalB1 + px*(1-py)*tmpvalB2 + (1-px)*py*tmpvalB3 + px*py*tmpvalB4;
				}else
				{
					frontR = 0;
					frontG = 0;
					frontB = 0;
				}

			}
			//rear
			if (rearweight < 0.001)
			{
				rearR = 0;
				rearG = 0;
				rearB = 0;
			}else
			{
				srcx = *(rearcoordinatexMat+y*STITCH_IMAGE_WIDTH+x);
				srcy = *(rearcoordinateyMat+y*STITCH_IMAGE_WIDTH+x);
				if ((int)(srcx) >= 0 && (int)(srcx+1) < IMAGE_WIDTH &&
					(int)(srcy) >= 0 && (int)(srcy+1) < IMAGE_HEIGHT)
				{
					tmpvalB1 = *(rearImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4);
					tmpvalB2 = *(rearImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4);
					tmpvalB3 = *(rearImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4);
					tmpvalB4 = *(rearImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4);
					tmpvalG1 = *(rearImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+1);
					tmpvalG2 = *(rearImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+1);
					tmpvalG3 = *(rearImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+1);
					tmpvalG4 = *(rearImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+1);
					tmpvalR1 = *(rearImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+2);
					tmpvalR2 = *(rearImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+2);
					tmpvalR3 = *(rearImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+2);
					tmpvalR4 = *(rearImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+2);
					px = srcx-(int)(srcx);
					py = srcy-(int)(srcy);
					rearR = (1-px)*(1-py)*tmpvalR1 + px*(1-py)*tmpvalR2 + (1-px)*py*tmpvalR3 + px*py*tmpvalR4;
					rearG = (1-px)*(1-py)*tmpvalG1 + px*(1-py)*tmpvalG2 + (1-px)*py*tmpvalG3 + px*py*tmpvalG4;
					rearB = (1-px)*(1-py)*tmpvalB1 + px*(1-py)*tmpvalB2 + (1-px)*py*tmpvalB3 + px*py*tmpvalB4;
				}else
				{
					rearR = 0;
					rearG = 0;
					rearB = 0;
				}

			}
			//left
			if (leftweight < 0.001)
			{
				leftR = 0;
				leftG = 0;
				leftB = 0;
			}else
			{
				srcx = *(leftcoordinatexMat+y*STITCH_IMAGE_WIDTH+x);
				srcy = *(leftcoordinateyMat+y*STITCH_IMAGE_WIDTH+x);
				if ((int)(srcx) >= 0 && (int)(srcx+1) < IMAGE_WIDTH &&
					(int)(srcy) >= 0 && (int)(srcy+1) < IMAGE_HEIGHT)
				{
					tmpvalB1 = *(leftImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4);
					tmpvalB2 = *(leftImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4);
					tmpvalB3 = *(leftImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4);
					tmpvalB4 = *(leftImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4);
					tmpvalG1 = *(leftImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+1);
					tmpvalG2 = *(leftImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+1);
					tmpvalG3 = *(leftImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+1);
					tmpvalG4 = *(leftImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+1);
					tmpvalR1 = *(leftImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+2);
					tmpvalR2 = *(leftImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+2);
					tmpvalR3 = *(leftImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+2);
					tmpvalR4 = *(leftImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+2);
					px = srcx-(int)(srcx);
					py = srcy-(int)(srcy);
					leftR = (1-px)*(1-py)*tmpvalR1 + px*(1-py)*tmpvalR2 + (1-px)*py*tmpvalR3 + px*py*tmpvalR4;
					leftG = (1-px)*(1-py)*tmpvalG1 + px*(1-py)*tmpvalG2 + (1-px)*py*tmpvalG3 + px*py*tmpvalG4;
					leftB = (1-px)*(1-py)*tmpvalB1 + px*(1-py)*tmpvalB2 + (1-px)*py*tmpvalB3 + px*py*tmpvalB4;
				}else
				{
					leftR = 0;
					leftG = 0;
					leftB = 0;
				}

			}
			//right
			if (rightweight < 0.001)
			{
				rightR = 0;
				rightG = 0;
				rightB = 0;
			}else
			{
				srcx = *(rightcoordinatexMat+y*STITCH_IMAGE_WIDTH+x);
				srcy = *(rightcoordinateyMat+y*STITCH_IMAGE_WIDTH+x);
				if ((int)(srcx) >= 0 && (int)(srcx+1) < IMAGE_WIDTH &&
					(int)(srcy) >= 0 && (int)(srcy+1) < IMAGE_HEIGHT)
				{
					tmpvalB1 = *(rightImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4);
					tmpvalB2 = *(rightImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4);
					tmpvalB3 = *(rightImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4);
					tmpvalB4 = *(rightImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4);
					tmpvalG1 = *(rightImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+1);
					tmpvalG2 = *(rightImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+1);
					tmpvalG3 = *(rightImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+1);
					tmpvalG4 = *(rightImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+1);
					tmpvalR1 = *(rightImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+2);
					tmpvalR2 = *(rightImage+(((int)(srcy-0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+2);
					tmpvalR3 = *(rightImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx-0.5)))*4+2);
					tmpvalR4 = *(rightImage+(((int)(srcy+0.5))*IMAGE_WIDTH+((int)(srcx+0.5)))*4+2);
					px = srcx-(int)(srcx);
					py = srcy-(int)(srcy);
					rightR = (1-px)*(1-py)*tmpvalR1 + px*(1-py)*tmpvalR2 + (1-px)*py*tmpvalR3 + px*py*tmpvalR4;
					rightG = (1-px)*(1-py)*tmpvalG1 + px*(1-py)*tmpvalG2 + (1-px)*py*tmpvalG3 + px*py*tmpvalG4;
					rightB = (1-px)*(1-py)*tmpvalB1 + px*(1-py)*tmpvalB2 + (1-px)*py*tmpvalB3 + px*py*tmpvalB4;
				}else
				{
					rightR = 0;
					rightG = 0;
					rightB = 0;
				}
			}
			//dstval
			*(dst)		= frontweight*frontB+rearweight*rearB+leftweight*leftB+rightweight*rightB;
			*(dst+1)	= frontweight*frontG+rearweight*rearG+leftweight*leftG+rightweight*rightG;
			*(dst+2)	= frontweight*frontR+rearweight*rearR+leftweight*leftR+rightweight*rightR;
			*(dst+3)	= 0;
			dst+=4;
		}
	}

	return true;
}
/************************************************************************/
/*				多线程调用(定点运算)									*/
/*              根据全景表生成全景图                                    */
/*				unsigned int *frontcoordinatexMat: 前向X坐标表			*/
/*				unsigned int *frontcoordinateyMat: 前向Y坐标表			*/
/*				unsigned int *frontweightMat:		前向权值表			*/
/*				unsigned int *rearcoordinatexMat:	后向X坐标表			*/
/*				unsigned int *rearcoordinateyMat:	后向Y坐标表			*/
/*				unsigned int *rearweightMat:		后向权值表			*/
/*				unsigned int *leftcoordinatexMat:	左向X坐标表			*/
/*				unsigned int *leftcoordinateyMat:	左向Y坐标表			*/
/*				unsigned int *leftweightMat:		左向权值表			*/
/*				unsigned int *rightcoordinatexMat: 右向X坐标表			*/
/*				unsigned int *rightcoordinateyMat: 右向Y坐标表			*/
/*				unsigned char *frontImage	前向源图像内存地址			*/
/*				unsigned char *rearImage	后向源图像内存地址			*/
/*				unsigned char *leftImage	左向源图像内存地址			*/
/*				unsigned char *rightImage	右向源图像内存地址			*/
/*				unsigned char *stitchImage	输出全景图像内存地址		*/
/*				int xoffset:				x坐标偏移量					*/
/*				int yoffset:				y坐标偏移量					*/
/*				int dstwidthstep:			显示到显示屏的宽			*/
/*				int NX:						水平方向被分成NX部分		*/
/*				int NY:						垂直方向被分成NY部分		*/
/************************************************************************/
bool GetStitchImage3(unsigned int *frontcoordinatexMat, unsigned int *frontcoordinateyMat, unsigned int *frontweightMat, unsigned int *rearcoordinatexMat, 
	unsigned int *rearcoordinateyMat, unsigned int *rearweightMat, unsigned int *leftcoordinatexMat, unsigned int *leftcoordinateyMat, unsigned int *leftweightMat, 
	unsigned int *rightcoordinatexMat, unsigned int *rightcoordinateyMat, unsigned int *rightweightMat, unsigned char *frontImage,  unsigned char *rearImage,  
	unsigned char *leftImage,  unsigned char *rightImage,  	unsigned char *stitchImage, int xoffset, int yoffset, int dstwidthstep, int NX, int NY)
{
	unsigned int	frontweight, rearweight, leftweight, rightweight;
	unsigned int	frontR, frontG, frontB, rearR, rearG, rearB, leftR, leftG, leftB, rightR, rightG, rightB;
	unsigned int	fronttmpvalR1, fronttmpvalR2, fronttmpvalR3, fronttmpvalR4;
	unsigned int	fronttmpvalG1, fronttmpvalG2, fronttmpvalG3, fronttmpvalG4;
	unsigned int	fronttmpvalB1, fronttmpvalB2, fronttmpvalB3, fronttmpvalB4;
	unsigned int	reartmpvalR1, reartmpvalR2, reartmpvalR3, reartmpvalR4;
	unsigned int	reartmpvalG1, reartmpvalG2, reartmpvalG3, reartmpvalG4;
	unsigned int	reartmpvalB1, reartmpvalB2, reartmpvalB3, reartmpvalB4;
	unsigned int	lefttmpvalR1, lefttmpvalR2, lefttmpvalR3, lefttmpvalR4;
	unsigned int	lefttmpvalG1, lefttmpvalG2, lefttmpvalG3, lefttmpvalG4;
	unsigned int	lefttmpvalB1, lefttmpvalB2, lefttmpvalB3, lefttmpvalB4;
	unsigned int	righttmpvalR1, righttmpvalR2, righttmpvalR3, righttmpvalR4;
	unsigned int	righttmpvalG1, righttmpvalG2, righttmpvalG3, righttmpvalG4;
	unsigned int	righttmpvalB1, righttmpvalB2, righttmpvalB3, righttmpvalB4;
	unsigned int	frontsrcx, frontsrcy, frontpx, frontpy;
	unsigned int	rearsrcx, rearsrcy, rearpx, rearpy;
	unsigned int	leftsrcx, leftsrcy, leftpx, leftpy;
	unsigned int	rightsrcx, rightsrcy, rightpx, rightpy;
	unsigned char	*dst; 
	unsigned int	tmpfrontx, tmpfronty, tmprearx, tmpreary, tmpleftx, tmplefty, tmprightx, tmprighty;
	int xend = xoffset+STITCH_IMAGE_WIDTH/NX;
	int yend = yoffset+STITCH_IMAGE_HEIGHT/NY;
	for (int y = yoffset; y < yend; ++y)
	{
		dst = stitchImage+(y*dstwidthstep+xoffset)*4;
		for (int x = xoffset; x < xend; ++x)
		{
			frontweight = frontweightMat[y*STITCH_IMAGE_WIDTH+x];
			rearweight	= rearweightMat[y*STITCH_IMAGE_WIDTH+x];
			leftweight	= leftweightMat[y*STITCH_IMAGE_WIDTH+x];
			rightweight = rightweightMat[y*STITCH_IMAGE_WIDTH+x];
			//front
			if (frontweight < 1)
			{
				frontR = 0;
				frontG = 0;
				frontB = 0;
			}else
			{
				tmpfrontx = frontcoordinatexMat[y*STITCH_IMAGE_WIDTH+x];
				tmpfronty = frontcoordinateyMat[y*STITCH_IMAGE_WIDTH+x];
				frontsrcx = tmpfrontx >> ACCURACY_BIT;
				frontsrcy = tmpfronty >> ACCURACY_BIT;
				if (frontsrcx < IMAGE_WIDTH && frontsrcy < IMAGE_HEIGHT)
				{
					fronttmpvalB1 = *(frontImage+(frontsrcy*IMAGE_WIDTH+frontsrcx)*4);
					fronttmpvalB2 = *(frontImage+(frontsrcy*IMAGE_WIDTH+frontsrcx+1)*4);
					fronttmpvalB3 = *(frontImage+((frontsrcy+1)*IMAGE_WIDTH+frontsrcx)*4);
					fronttmpvalB4 = *(frontImage+((frontsrcy+1)*IMAGE_WIDTH+frontsrcx+1)*4);
					fronttmpvalG1 = *(frontImage+(frontsrcy*IMAGE_WIDTH+frontsrcx)*4+1);
					fronttmpvalG2 = *(frontImage+(frontsrcy*IMAGE_WIDTH+frontsrcx+1)*4+1);
					fronttmpvalG3 = *(frontImage+((frontsrcy+1)*IMAGE_WIDTH+frontsrcx)*4+1);
					fronttmpvalG4 = *(frontImage+((frontsrcy+1)*IMAGE_WIDTH+frontsrcx+1)*4+1);
					fronttmpvalR1 = *(frontImage+(frontsrcy*IMAGE_WIDTH+frontsrcx)*4+2);
					fronttmpvalR2 = *(frontImage+(frontsrcy*IMAGE_WIDTH+frontsrcx+1)*4+2);
					fronttmpvalR3 = *(frontImage+((frontsrcy+1)*IMAGE_WIDTH+frontsrcx)*4+2);
					fronttmpvalR4 = *(frontImage+((frontsrcy+1)*IMAGE_WIDTH+frontsrcx+1)*4+2);

					frontpx = tmpfrontx&ACCURACY_POINT;
					frontpy = tmpfronty&ACCURACY_POINT;
					frontR = ((ACCURACY_NUM-frontpx)*(ACCURACY_NUM-frontpy)*fronttmpvalR1+frontpx*(ACCURACY_NUM-frontpy)*fronttmpvalR2+
						(ACCURACY_NUM-frontpx)*frontpy*fronttmpvalR3+frontpx*frontpy*fronttmpvalR4)>>(ACCURACY_BIT<<1);
					frontG = ((ACCURACY_NUM-frontpx)*(ACCURACY_NUM-frontpy)*fronttmpvalG1+frontpx*(ACCURACY_NUM-frontpy)*fronttmpvalG2+
						(ACCURACY_NUM-frontpx)*frontpy*fronttmpvalG3+frontpx*frontpy*fronttmpvalG4)>>(ACCURACY_BIT<<1);
					frontB = ((ACCURACY_NUM-frontpx)*(ACCURACY_NUM-frontpy)*fronttmpvalB1+frontpx*(ACCURACY_NUM-frontpy)*fronttmpvalB2+
						(ACCURACY_NUM-frontpx)*frontpy*fronttmpvalB3+frontpx*frontpy*fronttmpvalB4)>>(ACCURACY_BIT<<1);
				}else
				{
					frontR = 0;
					frontG = 0;
					frontB = 0;
				}
			}
			//rear
			if (rearweight < 1)
			{
				rearR = 0;
				rearG = 0;
				rearB = 0;
			}else
			{
				tmpfrontx = rearcoordinatexMat[y*STITCH_IMAGE_WIDTH+x];
				tmpfronty = rearcoordinateyMat[y*STITCH_IMAGE_WIDTH+x];
				rearsrcx = tmpfrontx >> ACCURACY_BIT;
				rearsrcy = tmpfronty >> ACCURACY_BIT;
				if (rearsrcx < IMAGE_WIDTH && rearsrcy < IMAGE_HEIGHT)
				{
					reartmpvalB1 = *(rearImage+(rearsrcy*IMAGE_WIDTH+rearsrcx)*4);
					reartmpvalB2 = *(rearImage+(rearsrcy*IMAGE_WIDTH+rearsrcx+1)*4);
					reartmpvalB3 = *(rearImage+((rearsrcy+1)*IMAGE_WIDTH+rearsrcx)*4);
					reartmpvalB4 = *(rearImage+((rearsrcy+1)*IMAGE_WIDTH+rearsrcx+1)*4);
					reartmpvalG1 = *(rearImage+(rearsrcy*IMAGE_WIDTH+rearsrcx)*4+1);
					reartmpvalG2 = *(rearImage+(rearsrcy*IMAGE_WIDTH+rearsrcx+1)*4+1);
					reartmpvalG3 = *(rearImage+((rearsrcy+1)*IMAGE_WIDTH+rearsrcx)*4+1);
					reartmpvalG4 = *(rearImage+((rearsrcy+1)*IMAGE_WIDTH+rearsrcx+1)*4+1);
					reartmpvalR1 = *(rearImage+(rearsrcy*IMAGE_WIDTH+rearsrcx)*4+2);
					reartmpvalR2 = *(rearImage+(rearsrcy*IMAGE_WIDTH+rearsrcx+1)*4+2);
					reartmpvalR3 = *(rearImage+((rearsrcy+1)*IMAGE_WIDTH+rearsrcx)*4+2);
					reartmpvalR4 = *(rearImage+((rearsrcy+1)*IMAGE_WIDTH+rearsrcx+1)*4+2);

					rearpx = tmpfrontx&ACCURACY_POINT;
					rearpy = tmpfronty&ACCURACY_POINT;
					rearR = ((ACCURACY_NUM-rearpx)*(ACCURACY_NUM-rearpy)*reartmpvalR1+rearpx*(ACCURACY_NUM-rearpy)*reartmpvalR2+
						(ACCURACY_NUM-rearpx)*rearpy*reartmpvalR3+rearpx*rearpy*reartmpvalR4)>>(ACCURACY_BIT<<1);
					rearG = ((ACCURACY_NUM-rearpx)*(ACCURACY_NUM-rearpy)*reartmpvalG1+rearpx*(ACCURACY_NUM-rearpy)*reartmpvalG2+
						(ACCURACY_NUM-rearpx)*rearpy*reartmpvalG3+rearpx*rearpy*reartmpvalG4)>>(ACCURACY_BIT<<1);
					rearB = ((ACCURACY_NUM-rearpx)*(ACCURACY_NUM-rearpy)*reartmpvalB1+rearpx*(ACCURACY_NUM-rearpy)*reartmpvalB2+
						(ACCURACY_NUM-rearpx)*rearpy*reartmpvalB3+rearpx*rearpy*reartmpvalB4)>>(ACCURACY_BIT<<1);
				}else
				{
					rearR = 0;
					rearG = 0;
					rearB = 0;
				}

			}
			//left
			if (leftweight < 1)
			{
				leftR = 0;
				leftG = 0;
				leftB = 0;
			}else
			{
				tmpfrontx = leftcoordinatexMat[y*STITCH_IMAGE_WIDTH+x];
				tmpfronty = leftcoordinateyMat[y*STITCH_IMAGE_WIDTH+x];
				leftsrcx = tmpfrontx >> ACCURACY_BIT;
				leftsrcy = tmpfronty >> ACCURACY_BIT;
				if (leftsrcx < IMAGE_WIDTH && leftsrcy < IMAGE_HEIGHT)
				{
					lefttmpvalB1 = *(leftImage+(leftsrcy*IMAGE_WIDTH+leftsrcx)*4);
					lefttmpvalB2 = *(leftImage+(leftsrcy*IMAGE_WIDTH+leftsrcx+1)*4);
					lefttmpvalB3 = *(leftImage+((leftsrcy+1)*IMAGE_WIDTH+leftsrcx)*4);
					lefttmpvalB4 = *(leftImage+((leftsrcy+1)*IMAGE_WIDTH+leftsrcx+1)*4);
					lefttmpvalG1 = *(leftImage+(leftsrcy*IMAGE_WIDTH+leftsrcx)*4+1);
					lefttmpvalG2 = *(leftImage+(leftsrcy*IMAGE_WIDTH+leftsrcx+1)*4+1);
					lefttmpvalG3 = *(leftImage+((leftsrcy+1)*IMAGE_WIDTH+leftsrcx)*4+1);
					lefttmpvalG4 = *(leftImage+((leftsrcy+1)*IMAGE_WIDTH+leftsrcx+1)*4+1);
					lefttmpvalR1 = *(leftImage+(leftsrcy*IMAGE_WIDTH+leftsrcx)*4+2);
					lefttmpvalR2 = *(leftImage+(leftsrcy*IMAGE_WIDTH+leftsrcx+1)*4+2);
					lefttmpvalR3 = *(leftImage+((leftsrcy+1)*IMAGE_WIDTH+leftsrcx)*4+2);
					lefttmpvalR4 = *(leftImage+((leftsrcy+1)*IMAGE_WIDTH+leftsrcx+1)*4+2);

					leftpx = tmpfrontx&ACCURACY_POINT;
					leftpy = tmpfronty&ACCURACY_POINT;
					leftR = ((ACCURACY_NUM-leftpx)*(ACCURACY_NUM-leftpy)*lefttmpvalR1+leftpx*(ACCURACY_NUM-leftpy)*lefttmpvalR2+
						(ACCURACY_NUM-leftpx)*leftpy*lefttmpvalR3+leftpx*leftpy*lefttmpvalR4)>>(ACCURACY_BIT<<1);
					leftG = ((ACCURACY_NUM-leftpx)*(ACCURACY_NUM-leftpy)*lefttmpvalG1+leftpx*(ACCURACY_NUM-leftpy)*lefttmpvalG2+
						(ACCURACY_NUM-leftpx)*leftpy*lefttmpvalG3+leftpx*leftpy*lefttmpvalG4)>>(ACCURACY_BIT<<1);
					leftB = ((ACCURACY_NUM-leftpx)*(ACCURACY_NUM-leftpy)*lefttmpvalB1+leftpx*(ACCURACY_NUM-leftpy)*lefttmpvalB2+
						(ACCURACY_NUM-leftpx)*leftpy*lefttmpvalB3+leftpx*leftpy*lefttmpvalB4)>>(ACCURACY_BIT<<1);
				}else
				{
					leftR = 0;
					leftG = 0;
					leftB = 0;
				}

			}
			//right
			if (rightweight < 1)
			{
				rightR = 0;
				rightG = 0;
				rightB = 0;
			}else
			{
				tmpfrontx = rightcoordinatexMat[y*STITCH_IMAGE_WIDTH+x];
				tmpfronty = rightcoordinateyMat[y*STITCH_IMAGE_WIDTH+x];
				rightsrcx = tmpfrontx >> ACCURACY_BIT;
				rightsrcy = tmpfronty >> ACCURACY_BIT;
				if (rightsrcx < IMAGE_WIDTH && rightsrcy < IMAGE_HEIGHT)
				{
					righttmpvalB1 = *(rightImage+(rightsrcy*IMAGE_WIDTH+rightsrcx)*4);
					righttmpvalB2 = *(rightImage+(rightsrcy*IMAGE_WIDTH+rightsrcx+1)*4);
					righttmpvalB3 = *(rightImage+((rightsrcy+1)*IMAGE_WIDTH+rightsrcx)*4);
					righttmpvalB4 = *(rightImage+((rightsrcy+1)*IMAGE_WIDTH+rightsrcx+1)*4);
					righttmpvalG1 = *(rightImage+(rightsrcy*IMAGE_WIDTH+rightsrcx)*4+1);
					righttmpvalG2 = *(rightImage+(rightsrcy*IMAGE_WIDTH+rightsrcx+1)*4+1);
					righttmpvalG3 = *(rightImage+((rightsrcy+1)*IMAGE_WIDTH+rightsrcx)*4+1);
					righttmpvalG4 = *(rightImage+((rightsrcy+1)*IMAGE_WIDTH+rightsrcx+1)*4+1);
					righttmpvalR1 = *(rightImage+(rightsrcy*IMAGE_WIDTH+rightsrcx)*4+2);
					righttmpvalR2 = *(rightImage+(rightsrcy*IMAGE_WIDTH+rightsrcx+1)*4+2);
					righttmpvalR3 = *(rightImage+((rightsrcy+1)*IMAGE_WIDTH+rightsrcx)*4+2);
					righttmpvalR4 = *(rightImage+((rightsrcy+1)*IMAGE_WIDTH+rightsrcx+1)*4+2);

					rightpx = tmpfrontx&ACCURACY_POINT;
					rightpy = tmpfronty&ACCURACY_POINT;
					rightR = ((ACCURACY_NUM-rightpx)*(ACCURACY_NUM-rightpy)*righttmpvalR1+rightpx*(ACCURACY_NUM-rightpy)*righttmpvalR2+
						(ACCURACY_NUM-rightpx)*rightpy*righttmpvalR3+rightpx*rightpy*righttmpvalR4)>>(ACCURACY_BIT<<1);
					rightG = ((ACCURACY_NUM-rightpx)*(ACCURACY_NUM-rightpy)*righttmpvalG1+rightpx*(ACCURACY_NUM-rightpy)*righttmpvalG2+
						(ACCURACY_NUM-rightpx)*rightpy*righttmpvalG3+rightpx*rightpy*righttmpvalG4)>>(ACCURACY_BIT<<1);
					rightB = ((ACCURACY_NUM-rightpx)*(ACCURACY_NUM-rightpy)*righttmpvalB1+rightpx*(ACCURACY_NUM-rightpy)*righttmpvalB2+
						(ACCURACY_NUM-rightpx)*rightpy*righttmpvalB3+rightpx*rightpy*righttmpvalB4)>>(ACCURACY_BIT<<1);
				}else
				{
					rightR = 0;
					rightG = 0;
					rightB = 0;
				}
			}
			//dstval
			*(dst)		= (frontweight*frontB+rearweight*rearB+leftweight*leftB+rightweight*rightB)>>ACCURACY_BIT;
			*(dst+1)	= (frontweight*frontG+rearweight*rearG+leftweight*leftG+rightweight*rightG)>>ACCURACY_BIT;
			*(dst+2)	= (frontweight*frontR+rearweight*rearR+leftweight*leftR+rightweight*rightR)>>ACCURACY_BIT;
			*(dst+3)	= 0;
			dst+=4;
		}
	}

	return true;
}
/************************************************************************/
/*				多线程调用(定点运算、一层循环)							*/
/*              根据全景表生成全景图                                    */
/*				unsigned int *frontcoordinatexMat:	前向X坐标表			*/
/*				unsigned int *frontcoordinateyMat:	前向Y坐标表			*/
/*				unsigned int *frontweightMat:		前向权值表			*/
/*				unsigned int *rearcoordinatexMat:	后向X坐标表			*/
/*				unsigned int *rearcoordinateyMat:	后向Y坐标表			*/
/*				unsigned int *rearweightMat:		后向权值表			*/
/*				unsigned int *leftcoordinatexMat:	左向X坐标表			*/
/*				unsigned int *leftcoordinateyMat:	左向Y坐标表			*/
/*				unsigned int *leftweightMat:		左向权值表			*/
/*				unsigned int *rightcoordinatexMat:	右向X坐标表			*/
/*				unsigned int *rightcoordinateyMat:	右向Y坐标表			*/
/*				unsigned char *frontImage			前向源图像内存地址	*/
/*				unsigned char *rearImage			后向源图像内存地址	*/
/*				unsigned char *leftImage			左向源图像内存地址	*/
/*				unsigned char *rightImage			右向源图像内存地址	*/
/*				unsigned char *stitchImage		输出全景图像内存首地址	*/
/*				int	yoffset:					垂直方向偏移量			*/
/*				int length:						执行拼接行数			*/
/*				int dstwidthstep:				显示屏的宽				*/
/************************************************************************/
bool GetStitchImage4(unsigned int *frontcoordinatexMat, unsigned int *frontcoordinateyMat, unsigned int *frontweightMat, unsigned int *rearcoordinatexMat, 
	unsigned int *rearcoordinateyMat, unsigned int *rearweightMat, unsigned int *leftcoordinatexMat, unsigned int *leftcoordinateyMat, unsigned int *leftweightMat, 
	unsigned int *rightcoordinatexMat, unsigned int *rightcoordinateyMat, unsigned int *rightweightMat, unsigned char *frontImage,  unsigned char *rearImage,  
	unsigned char *leftImage,  unsigned char *rightImage,  	unsigned char *stitchImage, int yoffset, int length, int dstwidthstep)
{
	unsigned int	frontweight, rearweight, leftweight, rightweight;
	unsigned int	frontR, frontG, frontB, rearR, rearG, rearB, leftR, leftG, leftB, rightR, rightG, rightB;
	unsigned int	fronttmpvalR1, fronttmpvalR2, fronttmpvalR3, fronttmpvalR4;
	unsigned int	fronttmpvalG1, fronttmpvalG2, fronttmpvalG3, fronttmpvalG4;
	unsigned int	fronttmpvalB1, fronttmpvalB2, fronttmpvalB3, fronttmpvalB4;
	unsigned int	reartmpvalR1, reartmpvalR2, reartmpvalR3, reartmpvalR4;
	unsigned int	reartmpvalG1, reartmpvalG2, reartmpvalG3, reartmpvalG4;
	unsigned int	reartmpvalB1, reartmpvalB2, reartmpvalB3, reartmpvalB4;
	unsigned int	lefttmpvalR1, lefttmpvalR2, lefttmpvalR3, lefttmpvalR4;
	unsigned int	lefttmpvalG1, lefttmpvalG2, lefttmpvalG3, lefttmpvalG4;
	unsigned int	lefttmpvalB1, lefttmpvalB2, lefttmpvalB3, lefttmpvalB4;
	unsigned int	righttmpvalR1, righttmpvalR2, righttmpvalR3, righttmpvalR4;
	unsigned int	righttmpvalG1, righttmpvalG2, righttmpvalG3, righttmpvalG4;
	unsigned int	righttmpvalB1, righttmpvalB2, righttmpvalB3, righttmpvalB4;
	unsigned int	frontsrcx, frontsrcy, frontpx, frontpy;
	unsigned int	rearsrcx, rearsrcy, rearpx, rearpy;
	unsigned int	leftsrcx, leftsrcy, leftpx, leftpy;
	unsigned int	rightsrcx, rightsrcy, rightpx, rightpy; 
	unsigned int	tmpfrontx, tmpfronty, tmprearx, tmpreary, tmpleftx, tmplefty, tmprightx, tmprighty;

	unsigned char	*dst = stitchImage+yoffset*dstwidthstep*4;
	int end = (yoffset + length)*STITCH_IMAGE_WIDTH;
	for (int i = yoffset*STITCH_IMAGE_WIDTH; i < end; ++i)
	{
		//front
		frontweight = frontweightMat[i];
		if (frontweight < 1)
		{
			frontR = 0;
			frontG = 0;
			frontB = 0;
		}else
		{
			tmpfrontx = frontcoordinatexMat[i];
			tmpfronty = frontcoordinateyMat[i];
			frontsrcx = tmpfrontx >> ACCURACY_BIT;
			frontsrcy = tmpfronty >> ACCURACY_BIT;
			if (frontsrcx < IMAGE_WIDTH && frontsrcy < IMAGE_HEIGHT)
			{
				fronttmpvalB1 = *(frontImage+(frontsrcy*IMAGE_WIDTH+frontsrcx)*4);
				fronttmpvalB2 = *(frontImage+(frontsrcy*IMAGE_WIDTH+frontsrcx+1)*4);
				fronttmpvalB3 = *(frontImage+((frontsrcy+1)*IMAGE_WIDTH+frontsrcx)*4);
				fronttmpvalB4 = *(frontImage+((frontsrcy+1)*IMAGE_WIDTH+frontsrcx+1)*4);
				fronttmpvalG1 = *(frontImage+(frontsrcy*IMAGE_WIDTH+frontsrcx)*4+1);
				fronttmpvalG2 = *(frontImage+(frontsrcy*IMAGE_WIDTH+frontsrcx+1)*4+1);
				fronttmpvalG3 = *(frontImage+((frontsrcy+1)*IMAGE_WIDTH+frontsrcx)*4+1);
				fronttmpvalG4 = *(frontImage+((frontsrcy+1)*IMAGE_WIDTH+frontsrcx+1)*4+1);
				fronttmpvalR1 = *(frontImage+(frontsrcy*IMAGE_WIDTH+frontsrcx)*4+2);
				fronttmpvalR2 = *(frontImage+(frontsrcy*IMAGE_WIDTH+frontsrcx+1)*4+2);
				fronttmpvalR3 = *(frontImage+((frontsrcy+1)*IMAGE_WIDTH+frontsrcx)*4+2);
				fronttmpvalR4 = *(frontImage+((frontsrcy+1)*IMAGE_WIDTH+frontsrcx+1)*4+2);

				frontpx = tmpfrontx&0x000003FF;
				frontpy = tmpfronty&0x000003FF;
				frontR = ((1024-frontpx)*(1024-frontpy)*fronttmpvalR1+frontpx*(1024-frontpy)*fronttmpvalR2+(1024-frontpx)*frontpy*fronttmpvalR3+frontpx*frontpy*fronttmpvalR4)>>(ACCURACY_BIT*2);
				frontG = ((1024-frontpx)*(1024-frontpy)*fronttmpvalG1+frontpx*(1024-frontpy)*fronttmpvalG2+(1024-frontpx)*frontpy*fronttmpvalG3+frontpx*frontpy*fronttmpvalG4)>>(ACCURACY_BIT*2);
				frontB = ((1024-frontpx)*(1024-frontpy)*fronttmpvalB1+frontpx*(1024-frontpy)*fronttmpvalB2+(1024-frontpx)*frontpy*fronttmpvalB3+frontpx*frontpy*fronttmpvalB4)>>(ACCURACY_BIT*2);
			}else
			{
				frontR = 0;
				frontG = 0;
				frontB = 0;
			}
		}
		//rear
		rearweight	= rearweightMat[i];
		if (rearweight < 1)
		{
			rearR = 0;
			rearG = 0;
			rearB = 0;
		}else
		{
			tmpfrontx = rearcoordinatexMat[i];
			tmpfronty = rearcoordinateyMat[i];
			rearsrcx = tmpfrontx >> ACCURACY_BIT;
			rearsrcy = tmpfronty >> ACCURACY_BIT;
			if (rearsrcx < IMAGE_WIDTH && rearsrcy < IMAGE_HEIGHT)
			{
				reartmpvalB1 = *(rearImage+(rearsrcy*IMAGE_WIDTH+rearsrcx)*4);
				reartmpvalB2 = *(rearImage+(rearsrcy*IMAGE_WIDTH+rearsrcx+1)*4);
				reartmpvalB3 = *(rearImage+((rearsrcy+1)*IMAGE_WIDTH+rearsrcx)*4);
				reartmpvalB4 = *(rearImage+((rearsrcy+1)*IMAGE_WIDTH+rearsrcx+1)*4);
				reartmpvalG1 = *(rearImage+(rearsrcy*IMAGE_WIDTH+rearsrcx)*4+1);
				reartmpvalG2 = *(rearImage+(rearsrcy*IMAGE_WIDTH+rearsrcx+1)*4+1);
				reartmpvalG3 = *(rearImage+((rearsrcy+1)*IMAGE_WIDTH+rearsrcx)*4+1);
				reartmpvalG4 = *(rearImage+((rearsrcy+1)*IMAGE_WIDTH+rearsrcx+1)*4+1);
				reartmpvalR1 = *(rearImage+(rearsrcy*IMAGE_WIDTH+rearsrcx)*4+2);
				reartmpvalR2 = *(rearImage+(rearsrcy*IMAGE_WIDTH+rearsrcx+1)*4+2);
				reartmpvalR3 = *(rearImage+((rearsrcy+1)*IMAGE_WIDTH+rearsrcx)*4+2);
				reartmpvalR4 = *(rearImage+((rearsrcy+1)*IMAGE_WIDTH+rearsrcx+1)*4+2);

				rearpx = tmpfrontx&0x000003FF;
				rearpy = tmpfronty&0x000003FF;
				rearR = ((1024-rearpx)*(1024-rearpy)*reartmpvalR1+rearpx*(1024-rearpy)*reartmpvalR2+(1024-rearpx)*rearpy*reartmpvalR3+rearpx*rearpy*reartmpvalR4)>>(ACCURACY_BIT*2);
				rearG = ((1024-rearpx)*(1024-rearpy)*reartmpvalG1+rearpx*(1024-rearpy)*reartmpvalG2+(1024-rearpx)*rearpy*reartmpvalG3+rearpx*rearpy*reartmpvalG4)>>(ACCURACY_BIT*2);
				rearB = ((1024-rearpx)*(1024-rearpy)*reartmpvalB1+rearpx*(1024-rearpy)*reartmpvalB2+(1024-rearpx)*rearpy*reartmpvalB3+rearpx*rearpy*reartmpvalB4)>>(ACCURACY_BIT*2);
			}else
			{
				rearR = 0;
				rearG = 0;
				rearB = 0;
			}

		}
		//left
		leftweight	= leftweightMat[i];
		if (leftweight < 1)
		{
			leftR = 0;
			leftG = 0;
			leftB = 0;
		}else
		{
			tmpfrontx = leftcoordinatexMat[i];
			tmpfronty = leftcoordinateyMat[i];
			leftsrcx = tmpfrontx >> ACCURACY_BIT;
			leftsrcy = tmpfronty >> ACCURACY_BIT;
			if (leftsrcx < IMAGE_WIDTH && leftsrcy < IMAGE_HEIGHT)
			{
				lefttmpvalB1 = *(leftImage+(leftsrcy*IMAGE_WIDTH+leftsrcx)*4);
				lefttmpvalB2 = *(leftImage+(leftsrcy*IMAGE_WIDTH+leftsrcx+1)*4);
				lefttmpvalB3 = *(leftImage+((leftsrcy+1)*IMAGE_WIDTH+leftsrcx)*4);
				lefttmpvalB4 = *(leftImage+((leftsrcy+1)*IMAGE_WIDTH+leftsrcx+1)*4);
				lefttmpvalG1 = *(leftImage+(leftsrcy*IMAGE_WIDTH+leftsrcx)*4+1);
				lefttmpvalG2 = *(leftImage+(leftsrcy*IMAGE_WIDTH+leftsrcx+1)*4+1);
				lefttmpvalG3 = *(leftImage+((leftsrcy+1)*IMAGE_WIDTH+leftsrcx)*4+1);
				lefttmpvalG4 = *(leftImage+((leftsrcy+1)*IMAGE_WIDTH+leftsrcx+1)*4+1);
				lefttmpvalR1 = *(leftImage+(leftsrcy*IMAGE_WIDTH+leftsrcx)*4+2);
				lefttmpvalR2 = *(leftImage+(leftsrcy*IMAGE_WIDTH+leftsrcx+1)*4+2);
				lefttmpvalR3 = *(leftImage+((leftsrcy+1)*IMAGE_WIDTH+leftsrcx)*4+2);
				lefttmpvalR4 = *(leftImage+((leftsrcy+1)*IMAGE_WIDTH+leftsrcx+1)*4+2);

				leftpx = tmpfrontx&0x000003FF;
				leftpy = tmpfronty&0x000003FF;
				leftR = ((1024-leftpx)*(1024-leftpy)*lefttmpvalR1+leftpx*(1024-leftpy)*lefttmpvalR2+(1024-leftpx)*leftpy*lefttmpvalR3+leftpx*leftpy*lefttmpvalR4)>>(ACCURACY_BIT*2);
				leftG = ((1024-leftpx)*(1024-leftpy)*lefttmpvalG1+leftpx*(1024-leftpy)*lefttmpvalG2+(1024-leftpx)*leftpy*lefttmpvalG3+leftpx*leftpy*lefttmpvalG4)>>(ACCURACY_BIT*2);
				leftB = ((1024-leftpx)*(1024-leftpy)*lefttmpvalB1+leftpx*(1024-leftpy)*lefttmpvalB2+(1024-leftpx)*leftpy*lefttmpvalB3+leftpx*leftpy*lefttmpvalB4)>>(ACCURACY_BIT*2);
			}else
			{
				leftR = 0;
				leftG = 0;
				leftB = 0;
			}

		}
		//right
		rightweight = rightweightMat[i];
		if (rightweight < 1)
		{
			rightR = 0;
			rightG = 0;
			rightB = 0;
		}else
		{
			tmpfrontx = rightcoordinatexMat[i];
			tmpfronty = rightcoordinateyMat[i];
			rightsrcx = tmpfrontx >> ACCURACY_BIT;
			rightsrcy = tmpfronty >> ACCURACY_BIT;
			if (rightsrcx < IMAGE_WIDTH && rightsrcy < IMAGE_HEIGHT)
			{
				righttmpvalB1 = *(rightImage+(rightsrcy*IMAGE_WIDTH+rightsrcx)*4);
				righttmpvalB2 = *(rightImage+(rightsrcy*IMAGE_WIDTH+rightsrcx+1)*4);
				righttmpvalB3 = *(rightImage+((rightsrcy+1)*IMAGE_WIDTH+rightsrcx)*4);
				righttmpvalB4 = *(rightImage+((rightsrcy+1)*IMAGE_WIDTH+rightsrcx+1)*4);
				righttmpvalG1 = *(rightImage+(rightsrcy*IMAGE_WIDTH+rightsrcx)*4+1);
				righttmpvalG2 = *(rightImage+(rightsrcy*IMAGE_WIDTH+rightsrcx+1)*4+1);
				righttmpvalG3 = *(rightImage+((rightsrcy+1)*IMAGE_WIDTH+rightsrcx)*4+1);
				righttmpvalG4 = *(rightImage+((rightsrcy+1)*IMAGE_WIDTH+rightsrcx+1)*4+1);
				righttmpvalR1 = *(rightImage+(rightsrcy*IMAGE_WIDTH+rightsrcx)*4+2);
				righttmpvalR2 = *(rightImage+(rightsrcy*IMAGE_WIDTH+rightsrcx+1)*4+2);
				righttmpvalR3 = *(rightImage+((rightsrcy+1)*IMAGE_WIDTH+rightsrcx)*4+2);
				righttmpvalR4 = *(rightImage+((rightsrcy+1)*IMAGE_WIDTH+rightsrcx+1)*4+2);

				rightpx = tmpfrontx&0x000003FF;
				rightpy = tmpfronty&0x000003FF;
				rightR = ((1024-rightpx)*(1024-rightpy)*righttmpvalR1+rightpx*(1024-rightpy)*righttmpvalR2+(1024-rightpx)*rightpy*righttmpvalR3+rightpx*rightpy*righttmpvalR4)>>(ACCURACY_BIT*2);
				rightG = ((1024-rightpx)*(1024-rightpy)*righttmpvalG1+rightpx*(1024-rightpy)*righttmpvalG2+(1024-rightpx)*rightpy*righttmpvalG3+rightpx*rightpy*righttmpvalG4)>>(ACCURACY_BIT*2);
				rightB = ((1024-rightpx)*(1024-rightpy)*righttmpvalB1+rightpx*(1024-rightpy)*righttmpvalB2+(1024-rightpx)*rightpy*righttmpvalB3+rightpx*rightpy*righttmpvalB4)>>(ACCURACY_BIT*2);
			}else
			{
				rightR = 0;
				rightG = 0;
				rightB = 0;
			}
		}
		//dstval
		if (i%(STITCH_IMAGE_WIDTH*4) == 0)
		{
			dst += (dstwidthstep-STITCH_IMAGE_WIDTH)*4;
		}
		*dst++	= (frontweight*frontB+rearweight*rearB+leftweight*leftB+rightweight*rightB)>>ACCURACY_BIT;
		*dst++	= (frontweight*frontG+rearweight*rearG+leftweight*leftG+rightweight*rightG)>>ACCURACY_BIT;
		*dst++	= (frontweight*frontR+rearweight*rearR+leftweight*leftR+rightweight*rightR)>>ACCURACY_BIT;
		*dst++	= 0;
	}
	return true;
}
/************************************************************************/
/*				初始化拼接表(定点运算）                                 */
/*              根据全景表生成全景图                                    */
/*				unsigned int *frontcoordinatexMat: 前向X坐标表			*/
/*				unsigned int *frontcoordinateyMat: 前向Y坐标表			*/
/*				unsigned int *frontweightMat:		前向权值表			*/
/*				unsigned int *rearcoordinatexMat:	后向X坐标表			*/
/*				unsigned int *rearcoordinateyMat:	后向Y坐标表			*/
/*				unsigned int *rearweightMat:		后向权值表			*/
/*				unsigned int *leftcoordinatexMat:	左向X坐标表			*/
/*				unsigned int *leftcoordinateyMat:	左向Y坐标表			*/
/*				unsigned int *leftweightMat:		左向权值表			*/
/*				unsigned int *rightcoordinatexMat: 右向X坐标表			*/
/*				unsigned int *rightcoordinateyMat: 右向Y坐标表			*/
/************************************************************************/
bool InitStitchMap2(unsigned int **frontcoordinatex, unsigned int **frontcoordinatey, unsigned int **frontweight, unsigned int **rearcoordinatex, 
	unsigned int **rearcoordinatey, unsigned int **rearweight, unsigned int **leftcoordinatex, unsigned int **leftcoordinatey, unsigned int **leftweight, 
	unsigned int **rightcoordinatex, unsigned int **rightcoordinatey, unsigned int **rightweight)
{
	CvMat *frontcoordinatexMat		= (CvMat*)cvLoad(FRONT_COORDINATE_X_FILE_NAME);
	CvMat *frontcoordinateyMat		= (CvMat*)cvLoad(FRONT_COORDINATE_Y_FILE_NAME);
	CvMat *frontweightMat			= (CvMat*)cvLoad(FRONT_WEIGHT_FILE_NAME);
	CvMat *rearcoordinatexMat		= (CvMat*)cvLoad(REAR_COORDINATE_X_FILE_NAME);
	CvMat *rearcoordinateyMat		= (CvMat*)cvLoad(REAR_COORDINATE_Y_FILE_NAME);
	CvMat *rearweightMat			= (CvMat*)cvLoad(REAR_WEIGHT_FILE_NAME);
	CvMat *leftcoordinatexMat		= (CvMat*)cvLoad(LEFT_COORDINATE_X_FILE_NAME);
	CvMat *leftcoordinateyMat		= (CvMat*)cvLoad(LEFT_COORDINATE_Y_FILE_NAME);
	CvMat *leftweightMat			= (CvMat*)cvLoad(LEFT_WEIGHT_FILE_NAME);
	CvMat *rightcoordinatexMat		= (CvMat*)cvLoad(RIGHT_COORDINATE_X_FILE_NAME);
	CvMat *rightcoordinateyMat		= (CvMat*)cvLoad(RIGHT_COORDINATE_Y_FILE_NAME);
	CvMat *rightweightMat			= (CvMat*)cvLoad(RIGHT_WEIGHT_FILE_NAME);

	if (!frontcoordinatexMat || !frontcoordinateyMat || !frontweightMat || !rearcoordinatexMat || !rearcoordinateyMat || !rearweightMat || 
		!leftcoordinatexMat || !leftcoordinateyMat || !leftweightMat || !rightcoordinatexMat || !rightcoordinateyMat || !rightweightMat)
	{
		printf("读取拼接表失败\n");
		return false;
	}
	*frontcoordinatex	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*frontcoordinatey	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*frontweight		= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rearcoordinatex	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rearcoordinatey	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rearweight			= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*leftcoordinatex	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*leftcoordinatey	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*leftweight			= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rightcoordinatex	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rightcoordinatey	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rightweight		= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];

	for (int y = 0; y < STITCH_IMAGE_HEIGHT; ++y)
	{
		for (int x = 0; x < STITCH_IMAGE_WIDTH; ++x)
		{
			*(*frontcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*frontcoordinatexMat, float, y, x));
			*(*frontcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*frontcoordinateyMat, float, y, x));
			*(*frontweight+y*STITCH_IMAGE_WIDTH+x)		= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*frontweightMat, float, y, x));
			*(*rearcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*rearcoordinatexMat, float, y, x));
			*(*rearcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*rearcoordinateyMat, float, y, x));
			*(*rearweight+y*STITCH_IMAGE_WIDTH+x)		= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*rearweightMat, float, y, x));
			*(*leftcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*leftcoordinatexMat, float, y, x));
			*(*leftcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*leftcoordinateyMat, float, y, x));
			*(*leftweight+y*STITCH_IMAGE_WIDTH+x)		= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*leftweightMat, float, y, x));
			*(*rightcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*rightcoordinatexMat, float, y, x));
			*(*rightcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*rightcoordinateyMat, float, y, x));
			*(*rightweight+y*STITCH_IMAGE_WIDTH+x)		= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*rightweightMat, float, y, x));
		}
	}

	cvReleaseMat(&frontcoordinatexMat);
	cvReleaseMat(&frontcoordinateyMat);
	cvReleaseMat(&frontweightMat);
	cvReleaseMat(&rearcoordinatexMat);
	cvReleaseMat(&rearcoordinateyMat);
	cvReleaseMat(&rearweightMat);
	cvReleaseMat(&leftcoordinatexMat);
	cvReleaseMat(&leftcoordinateyMat);
	cvReleaseMat(&leftweightMat);
	cvReleaseMat(&rightcoordinatexMat);
	cvReleaseMat(&rightcoordinateyMat);
	cvReleaseMat(&rightweightMat);
	return true;
}
/************************************************************************/
/*				多线程调用(定点运算、不做插值)									*/
/*              根据全景表生成全景图                                    */
/*				unsigned int *frontcoordinatexMat: 前向X坐标表			*/
/*				unsigned int *frontcoordinateyMat: 前向Y坐标表			*/
/*				unsigned int *frontweightMat:		前向权值表			*/
/*				unsigned int *rearcoordinatexMat:	后向X坐标表			*/
/*				unsigned int *rearcoordinateyMat:	后向Y坐标表			*/
/*				unsigned int *rearweightMat:		后向权值表			*/
/*				unsigned int *leftcoordinatexMat:	左向X坐标表			*/
/*				unsigned int *leftcoordinateyMat:	左向Y坐标表			*/
/*				unsigned int *leftweightMat:		左向权值表			*/
/*				unsigned int *rightcoordinatexMat: 右向X坐标表			*/
/*				unsigned int *rightcoordinateyMat: 右向Y坐标表			*/
/*				unsigned char *frontImage	前向源图像内存地址			*/
/*				unsigned char *rearImage	后向源图像内存地址			*/
/*				unsigned char *leftImage	左向源图像内存地址			*/
/*				unsigned char *rightImage	右向源图像内存地址			*/
/*				unsigned char *stitchImage	输出全景图像内存地址		*/
/*				int xoffset:				x坐标偏移量					*/
/*				int yoffset:				y坐标偏移量					*/
/*				int dstwidthstep:			显示到显示屏的宽			*/
/*				int NX:						水平方向被分成NX部分		*/
/*				int NY:						垂直方向被分成NY部分		*/
/************************************************************************/
bool GetStitchImage5(unsigned int *frontcoordinatexMat, unsigned int *frontcoordinateyMat, unsigned int *frontweightMat, unsigned int *rearcoordinatexMat, 
	unsigned int *rearcoordinateyMat, unsigned int *rearweightMat, unsigned int *leftcoordinatexMat, unsigned int *leftcoordinateyMat, unsigned int *leftweightMat, 
	unsigned int *rightcoordinatexMat, unsigned int *rightcoordinateyMat, unsigned int *rightweightMat, unsigned char *frontImage,  unsigned char *rearImage,  
	unsigned char *leftImage,  unsigned char *rightImage,  	unsigned char *stitchImage, int xoffset, int yoffset, int dstwidthstep, int NX, int NY)
{
	unsigned int	frontweight, rearweight, leftweight, rightweight;
	unsigned int	frontR, frontG, frontB, rearR, rearG, rearB, leftR, leftG, leftB, rightR, rightG, rightB;
	unsigned int	frontsrcx, frontsrcy;
	unsigned int	rearsrcx, rearsrcy;
	unsigned int	leftsrcx, leftsrcy;
	unsigned int	rightsrcx, rightsrcy;
	unsigned char	*dst; 
	int xend = xoffset+STITCH_IMAGE_WIDTH/NX;
	int yend = yoffset+STITCH_IMAGE_HEIGHT/NY;
	int srcImageWidth = IMAGE_WIDTH*ACCURACY_NUM;
	int srcImageHeight = IMAGE_HEIGHT*ACCURACY_NUM;
	for (int y = yoffset; y < yend; ++y)
	{
		dst = stitchImage+(y*dstwidthstep+xoffset)*4;
		for (int x = xoffset; x < xend; ++x)
		{
			frontweight = frontweightMat[y*STITCH_IMAGE_WIDTH+x];
			rearweight	= rearweightMat[y*STITCH_IMAGE_WIDTH+x];
			leftweight	= leftweightMat[y*STITCH_IMAGE_WIDTH+x];
			rightweight = rightweightMat[y*STITCH_IMAGE_WIDTH+x];
			//front
			if (frontweight < 1)
			{
				frontR = 0;
				frontG = 0;
				frontB = 0;
			}else
			{
				frontsrcx = frontcoordinatexMat[y*STITCH_IMAGE_WIDTH+x];
				frontsrcy = frontcoordinateyMat[y*STITCH_IMAGE_WIDTH+x];
				if (frontsrcx < srcImageWidth && frontsrcy < srcImageHeight)
				{
					frontB = frontImage[(frontsrcy*IMAGE_WIDTH*ACCURACY_NUM+frontsrcx)<<2];
					frontG = frontImage[((frontsrcy*IMAGE_WIDTH*ACCURACY_NUM+frontsrcx)<<2)+1];					
					frontR = frontImage[((frontsrcy*IMAGE_WIDTH*ACCURACY_NUM+frontsrcx)<<2)+2];
				}else
				{
					frontR = 0;
					frontG = 0;
					frontB = 0;
				}
			}
			//rear
			if (rearweight < 1)
			{
				rearR = 0;
				rearG = 0;
				rearB = 0;
			}else
			{
				rearsrcx = rearcoordinatexMat[y*STITCH_IMAGE_WIDTH+x];
				rearsrcy = rearcoordinateyMat[y*STITCH_IMAGE_WIDTH+x];
				if (rearsrcx < srcImageWidth && rearsrcy < srcImageHeight)
				{
					rearB = rearImage[(rearsrcy*IMAGE_WIDTH*ACCURACY_NUM+rearsrcx)<<2];
					rearG = rearImage[((rearsrcy*IMAGE_WIDTH*ACCURACY_NUM+rearsrcx)<<2)+1];					
					rearR = rearImage[((rearsrcy*IMAGE_WIDTH*ACCURACY_NUM+rearsrcx)<<2)+2];
				}else
				{
					rearR = 0;
					rearG = 0;
					rearB = 0;
				}

			}
			//left
			if (leftweight < 1)
			{
				leftR = 0;
				leftG = 0;
				leftB = 0;
			}else
			{
				leftsrcx = leftcoordinatexMat[y*STITCH_IMAGE_WIDTH+x];
				leftsrcy = leftcoordinateyMat[y*STITCH_IMAGE_WIDTH+x];
				if (leftsrcx < srcImageWidth && leftsrcy < srcImageHeight)
				{
					leftB = leftImage[(leftsrcy*IMAGE_WIDTH*ACCURACY_NUM+leftsrcx)<<2];
					leftG = leftImage[((leftsrcy*IMAGE_WIDTH*ACCURACY_NUM+leftsrcx)<<2)+1];					
					leftR = leftImage[((leftsrcy*IMAGE_WIDTH*ACCURACY_NUM+leftsrcx)<<2)+2];
				}else
				{
					leftR = 0;
					leftG = 0;
					leftB = 0;
				}
			}
			//right
			if (rightweight < 1)
			{
				rightR = 0;
				rightG = 0;
				rightB = 0;
			}else
			{
				rightsrcx = rightcoordinatexMat[y*STITCH_IMAGE_WIDTH+x];
				rightsrcy = rightcoordinateyMat[y*STITCH_IMAGE_WIDTH+x];
				if (rightsrcx < srcImageWidth && rightsrcy < srcImageHeight)
				{
					rightB = rightImage[(rightsrcy*IMAGE_WIDTH*ACCURACY_NUM+rightsrcx)<<2];
					rightG = rightImage[((rightsrcy*IMAGE_WIDTH*ACCURACY_NUM+rightsrcx)<<2)+1];					
					rightR = rightImage[((rightsrcy*IMAGE_WIDTH*ACCURACY_NUM+rightsrcx)<<2)+2];
				}else
				{
					rightR = 0;
					rightG = 0;
					rightB = 0;
				}
			}
			//dstval
			*(dst)		= (frontweight*frontB+rearweight*rearB+leftweight*leftB+rightweight*rightB)>>ACCURACY_BIT;
			*(dst+1)	= (frontweight*frontG+rearweight*rearG+leftweight*leftG+rightweight*rightG)>>ACCURACY_BIT;
			*(dst+2)	= (frontweight*frontR+rearweight*rearR+leftweight*leftR+rightweight*rightR)>>ACCURACY_BIT;
			*(dst+3)	= 0;
			dst+=4;
		}
	}

	return true;
}
/************************************************************************/
/*				初始化拼接表(定点运算、不做插值、单幅源图像做处理）     */
/*              根据全景表生成全景图                                    */
/*				unsigned int *frontcoordinatexMat: 前向X坐标表			*/
/*				unsigned int *frontcoordinateyMat: 前向Y坐标表			*/
/*				unsigned int *frontweightMat:		前向权值表			*/
/*				unsigned int *rearcoordinatexMat:	后向X坐标表			*/
/*				unsigned int *rearcoordinateyMat:	后向Y坐标表			*/
/*				unsigned int *rearweightMat:		后向权值表			*/
/*				unsigned int *leftcoordinatexMat:	左向X坐标表			*/
/*				unsigned int *leftcoordinateyMat:	左向Y坐标表			*/
/*				unsigned int *leftweightMat:		左向权值表			*/
/*				unsigned int *rightcoordinatexMat: 右向X坐标表			*/
/*				unsigned int *rightcoordinateyMat: 右向Y坐标表			*/
/************************************************************************/
bool InitStitchMap3(unsigned int **frontcoordinatex, unsigned int **frontcoordinatey, unsigned int **frontweight, unsigned int **rearcoordinatex, 
	unsigned int **rearcoordinatey, unsigned int **rearweight, unsigned int **leftcoordinatex, unsigned int **leftcoordinatey, unsigned int **leftweight, 
	unsigned int **rightcoordinatex, unsigned int **rightcoordinatey, unsigned int **rightweight)
{
	CvMat *frontcoordinatexMat		= (CvMat*)cvLoad(FRONT_COORDINATE_X_FILE_NAME);
	CvMat *frontcoordinateyMat		= (CvMat*)cvLoad(FRONT_COORDINATE_Y_FILE_NAME);
	CvMat *frontweightMat			= (CvMat*)cvLoad(FRONT_WEIGHT_FILE_NAME);
	CvMat *rearcoordinatexMat		= (CvMat*)cvLoad(REAR_COORDINATE_X_FILE_NAME);
	CvMat *rearcoordinateyMat		= (CvMat*)cvLoad(REAR_COORDINATE_Y_FILE_NAME);
	CvMat *rearweightMat			= (CvMat*)cvLoad(REAR_WEIGHT_FILE_NAME);
	CvMat *leftcoordinatexMat		= (CvMat*)cvLoad(LEFT_COORDINATE_X_FILE_NAME);
	CvMat *leftcoordinateyMat		= (CvMat*)cvLoad(LEFT_COORDINATE_Y_FILE_NAME);
	CvMat *leftweightMat			= (CvMat*)cvLoad(LEFT_WEIGHT_FILE_NAME);
	CvMat *rightcoordinatexMat		= (CvMat*)cvLoad(RIGHT_COORDINATE_X_FILE_NAME);
	CvMat *rightcoordinateyMat		= (CvMat*)cvLoad(RIGHT_COORDINATE_Y_FILE_NAME);
	CvMat *rightweightMat			= (CvMat*)cvLoad(RIGHT_WEIGHT_FILE_NAME);

	if (!frontcoordinatexMat || !frontcoordinateyMat || !frontweightMat || !rearcoordinatexMat || !rearcoordinateyMat || !rearweightMat || 
		!leftcoordinatexMat || !leftcoordinateyMat || !leftweightMat || !rightcoordinatexMat || !rightcoordinateyMat || !rightweightMat)
	{
		printf("读取拼接表失败\n");
		return false;
	}
	*frontcoordinatex	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*frontcoordinatey	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*frontweight		= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rearcoordinatex	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rearcoordinatey	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT]; 
	*rearweight			= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*leftcoordinatex	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*leftcoordinatey	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*leftweight			= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rightcoordinatex	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rightcoordinatey	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rightweight		= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	//前1，后2，左3， 右4
	for (int y = 0; y < STITCH_IMAGE_HEIGHT; ++y)
	{
		for (int x = 0; x < STITCH_IMAGE_WIDTH; ++x)
		{
			*(*frontcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*frontcoordinatexMat, float, y, x));
			*(*frontcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*frontcoordinateyMat, float, y, x));
			*(*frontweight+y*STITCH_IMAGE_WIDTH+x)		= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*frontweightMat, float, y, x));
			*(*rearcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= ((unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)))+1440;
			*(*rearcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*rearcoordinateyMat, float, y, x));
			*(*rearweight+y*STITCH_IMAGE_WIDTH+x)		= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*rearweightMat, float, y, x));
			*(*leftcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*leftcoordinatexMat, float, y, x));
			*(*leftcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= ((unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*leftcoordinateyMat, float, y, x)))+960;
			*(*leftweight+y*STITCH_IMAGE_WIDTH+x)		= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*leftweightMat, float, y, x));
			*(*rightcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= ((unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)))+1440;
			*(*rightcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= ((unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)))+960;
			*(*rightweight+y*STITCH_IMAGE_WIDTH+x)		= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*rightweightMat, float, y, x));
		}
	}

	cvReleaseMat(&frontcoordinatexMat);
	cvReleaseMat(&frontcoordinateyMat);
	cvReleaseMat(&frontweightMat);
	cvReleaseMat(&rearcoordinatexMat);
	cvReleaseMat(&rearcoordinateyMat);
	cvReleaseMat(&rearweightMat);
	cvReleaseMat(&leftcoordinatexMat);
	cvReleaseMat(&leftcoordinateyMat);
	cvReleaseMat(&leftweightMat);
	cvReleaseMat(&rightcoordinatexMat);
	cvReleaseMat(&rightcoordinateyMat);
	cvReleaseMat(&rightweightMat);
	return true;
}
/************************************************************************/
/*				初始化拼接表(定点运算、不做插值、单幅源图像做处理，水平镜像） */
/*              根据全景表生成全景图                                    */
/*				unsigned int *frontcoordinatexMat: 前向X坐标表			*/
/*				unsigned int *frontcoordinateyMat: 前向Y坐标表			*/
/*				unsigned int *frontweightMat:		前向权值表			*/
/*				unsigned int *rearcoordinatexMat:	后向X坐标表			*/
/*				unsigned int *rearcoordinateyMat:	后向Y坐标表			*/
/*				unsigned int *rearweightMat:		后向权值表			*/
/*				unsigned int *leftcoordinatexMat:	左向X坐标表			*/
/*				unsigned int *leftcoordinateyMat:	左向Y坐标表			*/
/*				unsigned int *leftweightMat:		左向权值表			*/
/*				unsigned int *rightcoordinatexMat: 右向X坐标表			*/
/*				unsigned int *rightcoordinateyMat: 右向Y坐标表			*/
/************************************************************************/
bool InitStitchMap4(unsigned int **frontcoordinatex, unsigned int **frontcoordinatey, unsigned int **frontweight, unsigned int **rearcoordinatex, 
	unsigned int **rearcoordinatey, unsigned int **rearweight, unsigned int **leftcoordinatex, unsigned int **leftcoordinatey, unsigned int **leftweight, 
	unsigned int **rightcoordinatex, unsigned int **rightcoordinatey, unsigned int **rightweight)
{
	CvMat *frontcoordinatexMat		= (CvMat*)cvLoad(FRONT_COORDINATE_X_FILE_NAME);
	CvMat *frontcoordinateyMat		= (CvMat*)cvLoad(FRONT_COORDINATE_Y_FILE_NAME);
	CvMat *frontweightMat			= (CvMat*)cvLoad(FRONT_WEIGHT_FILE_NAME);
	CvMat *rearcoordinatexMat		= (CvMat*)cvLoad(REAR_COORDINATE_X_FILE_NAME);
	CvMat *rearcoordinateyMat		= (CvMat*)cvLoad(REAR_COORDINATE_Y_FILE_NAME);
	CvMat *rearweightMat			= (CvMat*)cvLoad(REAR_WEIGHT_FILE_NAME);
	CvMat *leftcoordinatexMat		= (CvMat*)cvLoad(LEFT_COORDINATE_X_FILE_NAME);
	CvMat *leftcoordinateyMat		= (CvMat*)cvLoad(LEFT_COORDINATE_Y_FILE_NAME);
	CvMat *leftweightMat			= (CvMat*)cvLoad(LEFT_WEIGHT_FILE_NAME);
	CvMat *rightcoordinatexMat		= (CvMat*)cvLoad(RIGHT_COORDINATE_X_FILE_NAME);
	CvMat *rightcoordinateyMat		= (CvMat*)cvLoad(RIGHT_COORDINATE_Y_FILE_NAME);
	CvMat *rightweightMat			= (CvMat*)cvLoad(RIGHT_WEIGHT_FILE_NAME);

	if (!frontcoordinatexMat || !frontcoordinateyMat || !frontweightMat || !rearcoordinatexMat || !rearcoordinateyMat || !rearweightMat || 
		!leftcoordinatexMat || !leftcoordinateyMat || !leftweightMat || !rightcoordinatexMat || !rightcoordinateyMat || !rightweightMat)
	{
		printf("读取拼接表失败\n");
		return false;
	}
	*frontcoordinatex	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*frontcoordinatey	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*frontweight		= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rearcoordinatex	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rearcoordinatey	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT]; 
	*rearweight			= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*leftcoordinatex	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*leftcoordinatey	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*leftweight			= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rightcoordinatex	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rightcoordinatey	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rightweight		= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	//前1，后2，左3， 右4
	for (int y = 0; y < STITCH_IMAGE_HEIGHT; ++y)
	{
		for (int x = 0; x < STITCH_IMAGE_WIDTH; ++x)
		{
			*(*frontcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*(720.-CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)));
			*(*frontcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*frontcoordinateyMat, float, y, x));
			*(*frontweight+y*STITCH_IMAGE_WIDTH+x)		= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*frontweightMat, float, y, x));
			*(*rearcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= ((unsigned int)(ACCURACY_NUM*(720.-CV_MAT_ELEM(*rearcoordinatexMat, float, y, x))))+1440;
			*(*rearcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*rearcoordinateyMat, float, y, x));
			*(*rearweight+y*STITCH_IMAGE_WIDTH+x)		= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*rearweightMat, float, y, x));
			*(*leftcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*(720.-CV_MAT_ELEM(*leftcoordinatexMat, float, y, x)));
			*(*leftcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= ((unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*leftcoordinateyMat, float, y, x)))+960;
			*(*leftweight+y*STITCH_IMAGE_WIDTH+x)		= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*leftweightMat, float, y, x));
			*(*rightcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= ((unsigned int)(ACCURACY_NUM*(720.-CV_MAT_ELEM(*rightcoordinatexMat, float, y, x))))+1440;
			*(*rightcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= ((unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)))+960;
			*(*rightweight+y*STITCH_IMAGE_WIDTH+x)		= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*rightweightMat, float, y, x));
		}
	}

	cvReleaseMat(&frontcoordinatexMat);
	cvReleaseMat(&frontcoordinateyMat);
	cvReleaseMat(&frontweightMat);
	cvReleaseMat(&rearcoordinatexMat);
	cvReleaseMat(&rearcoordinateyMat);
	cvReleaseMat(&rearweightMat);
	cvReleaseMat(&leftcoordinatexMat);
	cvReleaseMat(&leftcoordinateyMat);
	cvReleaseMat(&leftweightMat);
	cvReleaseMat(&rightcoordinatexMat);
	cvReleaseMat(&rightcoordinateyMat);
	cvReleaseMat(&rightweightMat);
	return true;
}



bool InitStitchMap4_s(unsigned int **frontcoordinatex, unsigned int **frontcoordinatey, unsigned int **frontweight, unsigned int **rearcoordinatex, 
	unsigned int **rearcoordinatey, unsigned int **rearweight, unsigned int **leftcoordinatex, unsigned int **leftcoordinatey, unsigned int **leftweight, 
	unsigned int **rightcoordinatex, unsigned int **rightcoordinatey, unsigned int **rightweight)
{
	CvMat *frontcoordinatexMat		= (CvMat*)cvLoad(FRONT_COORDINATE_X_FILE_NAME);
	CvMat *frontcoordinateyMat		= (CvMat*)cvLoad(FRONT_COORDINATE_Y_FILE_NAME);
	CvMat *frontweightMat			= (CvMat*)cvLoad(FRONT_WEIGHT_FILE_NAME);
	CvMat *rearcoordinatexMat		= (CvMat*)cvLoad(REAR_COORDINATE_X_FILE_NAME);
	CvMat *rearcoordinateyMat		= (CvMat*)cvLoad(REAR_COORDINATE_Y_FILE_NAME);
	CvMat *rearweightMat			= (CvMat*)cvLoad(REAR_WEIGHT_FILE_NAME);
	CvMat *leftcoordinatexMat		= (CvMat*)cvLoad(LEFT_COORDINATE_X_FILE_NAME);
	CvMat *leftcoordinateyMat		= (CvMat*)cvLoad(LEFT_COORDINATE_Y_FILE_NAME);
	CvMat *leftweightMat			= (CvMat*)cvLoad(LEFT_WEIGHT_FILE_NAME);
	CvMat *rightcoordinatexMat		= (CvMat*)cvLoad(RIGHT_COORDINATE_X_FILE_NAME);
	CvMat *rightcoordinateyMat		= (CvMat*)cvLoad(RIGHT_COORDINATE_Y_FILE_NAME);
	CvMat *rightweightMat			= (CvMat*)cvLoad(RIGHT_WEIGHT_FILE_NAME);

	if (!frontcoordinatexMat || !frontcoordinateyMat || !frontweightMat || !rearcoordinatexMat || !rearcoordinateyMat || !rearweightMat || 
		!leftcoordinatexMat || !leftcoordinateyMat || !leftweightMat || !rightcoordinatexMat || !rightcoordinateyMat || !rightweightMat)
	{
		printf("读取拼接表失败\n");
		return false;
	}
	*frontcoordinatex	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*frontcoordinatey	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*frontweight		= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rearcoordinatex	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rearcoordinatey	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT]; 
	*rearweight			= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*leftcoordinatex	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*leftcoordinatey	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*leftweight			= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rightcoordinatex	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rightcoordinatey	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	*rightweight		= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT];
	//前1，后2，左3， 右4
	for (int y = 0; y < STITCH_IMAGE_HEIGHT; ++y)
	{
		for (int x = 0; x < STITCH_IMAGE_WIDTH; ++x)
		{
			*(*frontcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*(720.-CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)));
			*(*frontcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*frontcoordinateyMat, float, y, x));
			*(*frontweight+y*STITCH_IMAGE_WIDTH+x)		= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*frontweightMat, float, y, x));
			*(*rearcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= ((unsigned int)(ACCURACY_NUM*(720.-CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)+720)));
			*(*rearcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*rearcoordinateyMat, float, y, x));
			*(*rearweight+y*STITCH_IMAGE_WIDTH+x)		= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*rearweightMat, float, y, x));
			*(*leftcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*(720.-CV_MAT_ELEM(*leftcoordinatexMat, float, y, x)));
			*(*leftcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= ((unsigned int)(ACCURACY_NUM*(CV_MAT_ELEM(*leftcoordinateyMat, float, y, x)+480)));
			*(*leftweight+y*STITCH_IMAGE_WIDTH+x)		= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*leftweightMat, float, y, x));
			*(*rightcoordinatex+y*STITCH_IMAGE_WIDTH+x)	= ((unsigned int)(ACCURACY_NUM*(720.-CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)+720)));
			*(*rightcoordinatey+y*STITCH_IMAGE_WIDTH+x)	= ((unsigned int)(ACCURACY_NUM*(CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)+480)));
			*(*rightweight+y*STITCH_IMAGE_WIDTH+x)		= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*rightweightMat, float, y, x));
		}
	}

	cvReleaseMat(&frontcoordinatexMat);
	cvReleaseMat(&frontcoordinateyMat);
	cvReleaseMat(&frontweightMat);
	cvReleaseMat(&rearcoordinatexMat);
	cvReleaseMat(&rearcoordinateyMat);
	cvReleaseMat(&rearweightMat);
	cvReleaseMat(&leftcoordinatexMat);
	cvReleaseMat(&leftcoordinateyMat);
	cvReleaseMat(&leftweightMat);
	cvReleaseMat(&rightcoordinatexMat);
	cvReleaseMat(&rightcoordinateyMat);
	cvReleaseMat(&rightweightMat);
	return true;
}


//modified by csq
bool InitStitchMap_c(unsigned int **front,unsigned int **rear,unsigned int **left,unsigned int **right)
{
	CvMat *frontcoordinatexMat		= (CvMat*)cvLoad(FRONT_COORDINATE_X_FILE_NAME);
	CvMat *frontcoordinateyMat		= (CvMat*)cvLoad(FRONT_COORDINATE_Y_FILE_NAME);
	CvMat *frontweightMat			= (CvMat*)cvLoad(FRONT_WEIGHT_FILE_NAME);
	CvMat *rearcoordinatexMat		= (CvMat*)cvLoad(REAR_COORDINATE_X_FILE_NAME);
	CvMat *rearcoordinateyMat		= (CvMat*)cvLoad(REAR_COORDINATE_Y_FILE_NAME);
	CvMat *rearweightMat			= (CvMat*)cvLoad(REAR_WEIGHT_FILE_NAME);
	CvMat *leftcoordinatexMat		= (CvMat*)cvLoad(LEFT_COORDINATE_X_FILE_NAME);
	CvMat *leftcoordinateyMat		= (CvMat*)cvLoad(LEFT_COORDINATE_Y_FILE_NAME);
	CvMat *leftweightMat			= (CvMat*)cvLoad(LEFT_WEIGHT_FILE_NAME);
	CvMat *rightcoordinatexMat		= (CvMat*)cvLoad(RIGHT_COORDINATE_X_FILE_NAME);
	CvMat *rightcoordinateyMat		= (CvMat*)cvLoad(RIGHT_COORDINATE_Y_FILE_NAME);
	CvMat *rightweightMat			= (CvMat*)cvLoad(RIGHT_WEIGHT_FILE_NAME);

	if (!frontcoordinatexMat || !frontcoordinateyMat || !frontweightMat || !rearcoordinatexMat || !rearcoordinateyMat || !rearweightMat || 
		!leftcoordinatexMat || !leftcoordinateyMat || !leftweightMat || !rightcoordinatexMat || !rightcoordinateyMat || !rightweightMat)
	{
		printf("读取拼接表失败\n");
		return false;
	}
	*front	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT*3];
	*rear	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT*3];
	*left	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT*3];
	*right	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT*3];
	
	//前1，后2，左3， 右4
	for (int y = 0; y < STITCH_IMAGE_HEIGHT; ++y)
	{
		for (int x = 0; x < STITCH_IMAGE_WIDTH; ++x)
		{
			*(*front+y*STITCH_IMAGE_WIDTH*3+x)			= (unsigned int)(ACCURACY_NUM*(720.-CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)));
			*(*front+y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*frontcoordinateyMat, float, y, x));
			*(*front+y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH*2+x)	= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*frontweightMat, float, y, x));
			*(*rear+y*STITCH_IMAGE_WIDTH*3+x)			= ((unsigned int)(ACCURACY_NUM*(720.-CV_MAT_ELEM(*rearcoordinatexMat, float, y, x))))+1440;
			*(*rear+y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*rearcoordinateyMat, float, y, x));
			*(*rear+y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH*2+x)	= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*rearweightMat, float, y, x));
			*(*left+y*STITCH_IMAGE_WIDTH*3+x)			= (unsigned int)(ACCURACY_NUM*(720.-CV_MAT_ELEM(*leftcoordinatexMat, float, y, x)));
			*(*left+y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH+x)	= ((unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*leftcoordinateyMat, float, y, x)))+960;
			*(*left+y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH*2+x)	= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*leftweightMat, float, y, x));
			*(*right+y*STITCH_IMAGE_WIDTH*3+x)			= ((unsigned int)(ACCURACY_NUM*(720.-CV_MAT_ELEM(*rightcoordinatexMat, float, y, x))))+1440;
			*(*right+y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH+x)	= ((unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)))+960;
			*(*right+y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH*2+x)			= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*rightweightMat, float, y, x));
		}
	}

	cvReleaseMat(&frontcoordinatexMat);
	cvReleaseMat(&frontcoordinateyMat);
	cvReleaseMat(&frontweightMat);
	cvReleaseMat(&rearcoordinatexMat);
	cvReleaseMat(&rearcoordinateyMat);
	cvReleaseMat(&rearweightMat);
	cvReleaseMat(&leftcoordinatexMat);
	cvReleaseMat(&leftcoordinateyMat);
	cvReleaseMat(&leftweightMat);
	cvReleaseMat(&rightcoordinatexMat);
	cvReleaseMat(&rightcoordinateyMat);
	cvReleaseMat(&rightweightMat);
	return true;
}

//add by csq
bool InitStitchMap_cs(unsigned int **front,unsigned int **rear,unsigned int **left,unsigned int **right)
{
	CvMat *frontcoordinatexMat		= (CvMat*)cvLoad(FRONT_COORDINATE_X_FILE_NAME);
	CvMat *frontcoordinateyMat		= (CvMat*)cvLoad(FRONT_COORDINATE_Y_FILE_NAME);
	CvMat *frontweightMat			= (CvMat*)cvLoad(FRONT_WEIGHT_FILE_NAME);
	CvMat *rearcoordinatexMat		= (CvMat*)cvLoad(REAR_COORDINATE_X_FILE_NAME);
	CvMat *rearcoordinateyMat		= (CvMat*)cvLoad(REAR_COORDINATE_Y_FILE_NAME);
	CvMat *rearweightMat			= (CvMat*)cvLoad(REAR_WEIGHT_FILE_NAME);
	CvMat *leftcoordinatexMat		= (CvMat*)cvLoad(LEFT_COORDINATE_X_FILE_NAME);
	CvMat *leftcoordinateyMat		= (CvMat*)cvLoad(LEFT_COORDINATE_Y_FILE_NAME);
	CvMat *leftweightMat			= (CvMat*)cvLoad(LEFT_WEIGHT_FILE_NAME);
	CvMat *rightcoordinatexMat		= (CvMat*)cvLoad(RIGHT_COORDINATE_X_FILE_NAME);
	CvMat *rightcoordinateyMat		= (CvMat*)cvLoad(RIGHT_COORDINATE_Y_FILE_NAME);
	CvMat *rightweightMat			= (CvMat*)cvLoad(RIGHT_WEIGHT_FILE_NAME);

	if (!frontcoordinatexMat || !frontcoordinateyMat || !frontweightMat || !rearcoordinatexMat || !rearcoordinateyMat || !rearweightMat || 
		!leftcoordinatexMat || !leftcoordinateyMat || !leftweightMat || !rightcoordinatexMat || !rightcoordinateyMat || !rightweightMat)
	{
		printf("读取拼接表失败\n");
		return false;
	}
	*front	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT*3];
	*rear	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT*3];
	*left	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT*3];
	*right	= new unsigned int[STITCH_IMAGE_WIDTH*STITCH_IMAGE_HEIGHT*3];
	
	//前1，后2，左3， 右4
	for (int y = 0; y < STITCH_IMAGE_HEIGHT; ++y)
	{
		for (int x = 0; x < STITCH_IMAGE_WIDTH; ++x)
		{
			*(*front+y*STITCH_IMAGE_WIDTH*3+x)			= (unsigned int)(ACCURACY_NUM*(720.-CV_MAT_ELEM(*frontcoordinatexMat, float, y, x)));
			*(*front+y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*frontcoordinateyMat, float, y, x));
			*(*front+y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH*2+x)	= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*frontweightMat, float, y, x));
			*(*rear+y*STITCH_IMAGE_WIDTH*3+x)			= ((unsigned int)(ACCURACY_NUM*(720.-CV_MAT_ELEM(*rearcoordinatexMat, float, y, x)+720)));
			*(*rear+y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH+x)	= (unsigned int)(ACCURACY_NUM*CV_MAT_ELEM(*rearcoordinateyMat, float, y, x));
			*(*rear+y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH*2+x)	= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*rearweightMat, float, y, x));
			*(*left+y*STITCH_IMAGE_WIDTH*3+x)			= (unsigned int)(ACCURACY_NUM*(720.-CV_MAT_ELEM(*leftcoordinatexMat, float, y, x)));
			*(*left+y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH+x)	= ((unsigned int)(ACCURACY_NUM*(CV_MAT_ELEM(*leftcoordinateyMat, float, y, x)+480)));
			*(*left+y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH*2+x)	= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*leftweightMat, float, y, x));
			*(*right+y*STITCH_IMAGE_WIDTH*3+x)			= ((unsigned int)(ACCURACY_NUM*(720.-CV_MAT_ELEM(*rightcoordinatexMat, float, y, x)+720)));
			*(*right+y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH+x)	= ((unsigned int)(ACCURACY_NUM*(CV_MAT_ELEM(*rightcoordinateyMat, float, y, x)+480)));
			*(*right+y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH*2+x)			= (unsigned int)(WeightAccuracNum*CV_MAT_ELEM(*rightweightMat, float, y, x));
		}
	}

	cvReleaseMat(&frontcoordinatexMat);
	cvReleaseMat(&frontcoordinateyMat);
	cvReleaseMat(&frontweightMat);
	cvReleaseMat(&rearcoordinatexMat);
	cvReleaseMat(&rearcoordinateyMat);
	cvReleaseMat(&rearweightMat);
	cvReleaseMat(&leftcoordinatexMat);
	cvReleaseMat(&leftcoordinateyMat);
	cvReleaseMat(&leftweightMat);
	cvReleaseMat(&rightcoordinatexMat);
	cvReleaseMat(&rightcoordinateyMat);
	cvReleaseMat(&rightweightMat);
	return true;
}

//add by csq
bool GetStitchImage_cs(unsigned int *front,unsigned int *rear,unsigned int *left ,unsigned int *right,unsigned char *srcImage, unsigned char *stitchImage, 
	int xoffset, int yoffset, int dstwidthstep, int NX, int NY)
{
	unsigned int	frontweight, rearweight, leftweight, rightweight;
	unsigned int	frontR, frontG, frontB, rearR, rearG, rearB, leftR, leftG, leftB, rightR, rightG, rightB;
	unsigned int	fronttmpvalR1, fronttmpvalR2, fronttmpvalR3, fronttmpvalR4;
	unsigned int	fronttmpvalG1, fronttmpvalG2, fronttmpvalG3, fronttmpvalG4;
	unsigned int	fronttmpvalB1, fronttmpvalB2, fronttmpvalB3, fronttmpvalB4;
	unsigned int	reartmpvalR1, reartmpvalR2, reartmpvalR3, reartmpvalR4;
	unsigned int	reartmpvalG1, reartmpvalG2, reartmpvalG3, reartmpvalG4;
	unsigned int	reartmpvalB1, reartmpvalB2, reartmpvalB3, reartmpvalB4;
	unsigned int	lefttmpvalR1, lefttmpvalR2, lefttmpvalR3, lefttmpvalR4;
	unsigned int	lefttmpvalG1, lefttmpvalG2, lefttmpvalG3, lefttmpvalG4;
	unsigned int	lefttmpvalB1, lefttmpvalB2, lefttmpvalB3, lefttmpvalB4;
	unsigned int	righttmpvalR1, righttmpvalR2, righttmpvalR3, righttmpvalR4;
	unsigned int	righttmpvalG1, righttmpvalG2, righttmpvalG3, righttmpvalG4;
	unsigned int	righttmpvalB1, righttmpvalB2, righttmpvalB3, righttmpvalB4;
	unsigned int	frontsrcx, frontsrcy, frontpx, frontpy;
	unsigned int	rearsrcx, rearsrcy, rearpx, rearpy;
	unsigned int	leftsrcx, leftsrcy, leftpx, leftpy;
	unsigned int	rightsrcx, rightsrcy, rightpx, rightpy;
	unsigned char	*dst; 
	unsigned int	tmpfrontx, tmpfronty, tmprearx, tmpreary, tmpleftx, tmplefty, tmprightx, tmprighty;
	int xend = xoffset+STITCH_IMAGE_WIDTH/NX;
	int yend = yoffset+STITCH_IMAGE_HEIGHT/NY;
	int srcImageWidth = 1440;
	int srcImageHeight = 960;
	int halfwidth = srcImageWidth/2;
	int halfheight = srcImageHeight/2;
	for (int y = yoffset; y < yend; ++y)
	{
		dst = stitchImage+(y*dstwidthstep+xoffset)*4;
		for (int x = xoffset; x < xend; ++x)
		{
			frontweight = front[y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH*2+x];
			rearweight	= rear[y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH*2+x];
			leftweight	= left[y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH*2+x];
			rightweight = right[y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH*2+x];
			//front
			if (frontweight < 1)
			{
				frontR = 0;
				frontG = 0;
				frontB = 0;
			}else
			{
				tmpfrontx = front[y*STITCH_IMAGE_WIDTH*3+x];
				tmpfronty = front[y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH+x];
				frontsrcx = tmpfrontx >> ACCURACY_BIT;
				frontsrcy = tmpfronty >> ACCURACY_BIT;
				if (frontsrcx < halfwidth && frontsrcy < halfheight)
				{
					fronttmpvalB1 = srcImage[(frontsrcy*1440+frontsrcx)*4];
					fronttmpvalB2 = srcImage[(frontsrcy*1440+frontsrcx+1)*4];
					fronttmpvalB3 = srcImage[((frontsrcy+1)*1440+frontsrcx)*4];
					fronttmpvalB4 = srcImage[((frontsrcy+1)*1440+frontsrcx+1)*4];
					fronttmpvalG1 = srcImage[(frontsrcy*1440+frontsrcx)*4+1];
					fronttmpvalG2 = srcImage[(frontsrcy*1440+frontsrcx+1)*4+1];
					fronttmpvalG3 = srcImage[((frontsrcy+1)*1440+frontsrcx)*4+1];
					fronttmpvalG4 = srcImage[((frontsrcy+1)*1440+frontsrcx+1)*4+1];
					fronttmpvalR1 = srcImage[(frontsrcy*1440+frontsrcx)*4+2];
					fronttmpvalR2 = srcImage[(frontsrcy*1440+frontsrcx+1)*4+2];
					fronttmpvalR3 = srcImage[((frontsrcy+1)*1440+frontsrcx)*4+2];
					fronttmpvalR4 = srcImage[((frontsrcy+1)*1440+frontsrcx+1)*4+2];

					frontpx = tmpfrontx&ACCURACY_POINT;
					frontpy = tmpfronty&ACCURACY_POINT;
					frontR = ((ACCURACY_NUM-frontpx)*(ACCURACY_NUM-frontpy)*fronttmpvalR1+frontpx*(ACCURACY_NUM-frontpy)*fronttmpvalR2+
						(ACCURACY_NUM-frontpx)*frontpy*fronttmpvalR3+frontpx*frontpy*fronttmpvalR4)>>(ACCURACY_BIT<<1);
					frontG = ((ACCURACY_NUM-frontpx)*(ACCURACY_NUM-frontpy)*fronttmpvalG1+frontpx*(ACCURACY_NUM-frontpy)*fronttmpvalG2+
						(ACCURACY_NUM-frontpx)*frontpy*fronttmpvalG3+frontpx*frontpy*fronttmpvalG4)>>(ACCURACY_BIT<<1);
					frontB = ((ACCURACY_NUM-frontpx)*(ACCURACY_NUM-frontpy)*fronttmpvalB1+frontpx*(ACCURACY_NUM-frontpy)*fronttmpvalB2+
						(ACCURACY_NUM-frontpx)*frontpy*fronttmpvalB3+frontpx*frontpy*fronttmpvalB4)>>(ACCURACY_BIT<<1);
				}else
				{
					frontR = 0;
					frontG = 0;
					frontB = 0;
				}
			}
			//rear
			if (rearweight < 1)
			{
				rearR = 0;
				rearG = 0;
				rearB = 0;
			}else
			{
				tmpfrontx = rear[y*STITCH_IMAGE_WIDTH*3+x];
				tmpfronty = rear[y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH+x];
				rearsrcx = tmpfrontx >> ACCURACY_BIT;
				rearsrcy = tmpfronty >> ACCURACY_BIT;
				if (rearsrcx > halfwidth && rearsrcx < srcImageWidth && rearsrcy < srcImageHeight)
				{
					reartmpvalB1 = srcImage[(rearsrcy*1440+rearsrcx)*4];
					reartmpvalB2 = srcImage[(rearsrcy*1440+rearsrcx+1)*4];
					reartmpvalB3 = srcImage[((rearsrcy+1)*1440+rearsrcx)*4];
					reartmpvalB4 = srcImage[((rearsrcy+1)*1440+rearsrcx+1)*4];
					reartmpvalG1 = srcImage[(rearsrcy*1440+rearsrcx)*4+1];
					reartmpvalG2 = srcImage[(rearsrcy*1440+rearsrcx+1)*4+1];
					reartmpvalG3 = srcImage[((rearsrcy+1)*1440+rearsrcx)*4+1];
					reartmpvalG4 = srcImage[((rearsrcy+1)*1440+rearsrcx+1)*4+1];
					reartmpvalR1 = srcImage[(rearsrcy*1440+rearsrcx)*4+2];
					reartmpvalR2 = srcImage[(rearsrcy*1440+rearsrcx+1)*4+2];
					reartmpvalR3 = srcImage[((rearsrcy+1)*1440+rearsrcx)*4+2];
					reartmpvalR4 = srcImage[((rearsrcy+1)*1440+rearsrcx+1)*4+2];

					rearpx = tmpfrontx&ACCURACY_POINT;
					rearpy = tmpfronty&ACCURACY_POINT;
					rearR = ((ACCURACY_NUM-rearpx)*(ACCURACY_NUM-rearpy)*reartmpvalR1+rearpx*(ACCURACY_NUM-rearpy)*reartmpvalR2+
						(ACCURACY_NUM-rearpx)*rearpy*reartmpvalR3+rearpx*rearpy*reartmpvalR4)>>(ACCURACY_BIT<<1);
					rearG = ((ACCURACY_NUM-rearpx)*(ACCURACY_NUM-rearpy)*reartmpvalG1+rearpx*(ACCURACY_NUM-rearpy)*reartmpvalG2+
						(ACCURACY_NUM-rearpx)*rearpy*reartmpvalG3+rearpx*rearpy*reartmpvalG4)>>(ACCURACY_BIT<<1);
					rearB = ((ACCURACY_NUM-rearpx)*(ACCURACY_NUM-rearpy)*reartmpvalB1+rearpx*(ACCURACY_NUM-rearpy)*reartmpvalB2+
						(ACCURACY_NUM-rearpx)*rearpy*reartmpvalB3+rearpx*rearpy*reartmpvalB4)>>(ACCURACY_BIT<<1);
				}else
				{
					rearR = 0;
					rearG = 0;
					rearB = 0;
				}

			}
			//left
			if (leftweight < 1)
			{
				leftR = 0;
				leftG = 0;
				leftB = 0;
			}else
			{
				tmpfrontx = left[y*STITCH_IMAGE_WIDTH*3+x];
				tmpfronty = left[y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH+x];
				leftsrcx = tmpfrontx >> ACCURACY_BIT;
				leftsrcy = tmpfronty >> ACCURACY_BIT;
				if (leftsrcx < halfwidth && leftsrcy > halfheight && leftsrcy < srcImageHeight)
				{
					lefttmpvalB1 = srcImage[(leftsrcy*1440+leftsrcx)*4];
					lefttmpvalB2 = srcImage[(leftsrcy*1440+leftsrcx+1)*4];
					lefttmpvalB3 = srcImage[((leftsrcy+1)*1440+leftsrcx)*4];
					lefttmpvalB4 = srcImage[((leftsrcy+1)*1440+leftsrcx+1)*4];
					lefttmpvalG1 = srcImage[(leftsrcy*1440+leftsrcx)*4+1];
					lefttmpvalG2 = srcImage[(leftsrcy*1440+leftsrcx+1)*4+1];
					lefttmpvalG3 = srcImage[((leftsrcy+1)*1440+leftsrcx)*4+1];
					lefttmpvalG4 = srcImage[((leftsrcy+1)*1440+leftsrcx+1)*4+1];
					lefttmpvalR1 = srcImage[(leftsrcy*1440+leftsrcx)*4+2];
					lefttmpvalR2 = srcImage[(leftsrcy*1440+leftsrcx+1)*4+2];
					lefttmpvalR3 = srcImage[((leftsrcy+1)*1440+leftsrcx)*4+2];
					lefttmpvalR4 = srcImage[((leftsrcy+1)*1440+leftsrcx+1)*4+2];

					leftpx = tmpfrontx&ACCURACY_POINT;
					leftpy = tmpfronty&ACCURACY_POINT;
					leftR = ((ACCURACY_NUM-leftpx)*(ACCURACY_NUM-leftpy)*lefttmpvalR1+leftpx*(ACCURACY_NUM-leftpy)*lefttmpvalR2+
						(ACCURACY_NUM-leftpx)*leftpy*lefttmpvalR3+leftpx*leftpy*lefttmpvalR4)>>(ACCURACY_BIT<<1);
					leftG = ((ACCURACY_NUM-leftpx)*(ACCURACY_NUM-leftpy)*lefttmpvalG1+leftpx*(ACCURACY_NUM-leftpy)*lefttmpvalG2+
						(ACCURACY_NUM-leftpx)*leftpy*lefttmpvalG3+leftpx*leftpy*lefttmpvalG4)>>(ACCURACY_BIT<<1);
					leftB = ((ACCURACY_NUM-leftpx)*(ACCURACY_NUM-leftpy)*lefttmpvalB1+leftpx*(ACCURACY_NUM-leftpy)*lefttmpvalB2+
						(ACCURACY_NUM-leftpx)*leftpy*lefttmpvalB3+leftpx*leftpy*lefttmpvalB4)>>(ACCURACY_BIT<<1);
				}else
				{
					leftR = 0;
					leftG = 0;
					leftB = 0;
				}

			}
			//right
			if (rightweight < 1)
			{
				rightR = 0;
				rightG = 0;
				rightB = 0;
			}else
			{
				tmpfrontx = right[y*STITCH_IMAGE_WIDTH*3+x];
				tmpfronty = right[y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH+x];
				rightsrcx = tmpfrontx >> ACCURACY_BIT;
				rightsrcy = tmpfronty >> ACCURACY_BIT;
				if (rightsrcx > halfwidth && rightsrcx < srcImageWidth && 
					rightsrcy > halfheight && rightsrcy < srcImageHeight)
				{
					righttmpvalB1 = srcImage[(rightsrcy*1440+rightsrcx)*4];
					righttmpvalB2 = srcImage[(rightsrcy*1440+rightsrcx+1)*4];
					righttmpvalB3 = srcImage[((rightsrcy+1)*1440+rightsrcx)*4];
					righttmpvalB4 = srcImage[((rightsrcy+1)*1440+rightsrcx+1)*4];
					righttmpvalG1 = srcImage[(rightsrcy*1440+rightsrcx)*4+1];
					righttmpvalG2 = srcImage[(rightsrcy*1440+rightsrcx+1)*4+1];
					righttmpvalG3 = srcImage[((rightsrcy+1)*1440+rightsrcx)*4+1];
					righttmpvalG4 = srcImage[((rightsrcy+1)*1440+rightsrcx+1)*4+1];
					righttmpvalR1 = srcImage[(rightsrcy*1440+rightsrcx)*4+2];
					righttmpvalR2 = srcImage[(rightsrcy*1440+rightsrcx+1)*4+2];
					righttmpvalR3 = srcImage[((rightsrcy+1)*1440+rightsrcx)*4+2];
					righttmpvalR4 = srcImage[((rightsrcy+1)*1440+rightsrcx+1)*4+2];

					rightpx = tmpfrontx&ACCURACY_POINT;
					rightpy = tmpfronty&ACCURACY_POINT;
					rightR = ((ACCURACY_NUM-rightpx)*(ACCURACY_NUM-rightpy)*righttmpvalR1+rightpx*(ACCURACY_NUM-rightpy)*righttmpvalR2+
						(ACCURACY_NUM-rightpx)*rightpy*righttmpvalR3+rightpx*rightpy*righttmpvalR4)>>(ACCURACY_BIT<<1);
					rightG = ((ACCURACY_NUM-rightpx)*(ACCURACY_NUM-rightpy)*righttmpvalG1+rightpx*(ACCURACY_NUM-rightpy)*righttmpvalG2+
						(ACCURACY_NUM-rightpx)*rightpy*righttmpvalG3+rightpx*rightpy*righttmpvalG4)>>(ACCURACY_BIT<<1);
					rightB = ((ACCURACY_NUM-rightpx)*(ACCURACY_NUM-rightpy)*righttmpvalB1+rightpx*(ACCURACY_NUM-rightpy)*righttmpvalB2+
						(ACCURACY_NUM-rightpx)*rightpy*righttmpvalB3+rightpx*rightpy*righttmpvalB4)>>(ACCURACY_BIT<<1);
				}else
				{
					rightR = 0;
					rightG = 0;
					rightB = 0;
				}
			}
			//dstval
			*(dst)		= (frontweight*frontB+rearweight*rearB+leftweight*leftB+rightweight*rightB)>>WeightAccuracyBit;
			*(dst+1)	= (frontweight*frontG+rearweight*rearG+leftweight*leftG+rightweight*rightG)>>WeightAccuracyBit;
			*(dst+2)	= (frontweight*frontR+rearweight*rearR+leftweight*leftR+rightweight*rightR)>>WeightAccuracyBit;
			*(dst+3)	= 0;
			dst+=4;
		}
	}

	return true;
}

//add by csq
bool GetStitchImage_c(unsigned int *front,unsigned int *rear,unsigned int *left,unsigned int *right,unsigned char *srcImage, unsigned char *stitchImage, 
	int xoffset, int yoffset, int dstwidthstep, int NX, int NY)
{
	unsigned int	frontweight, rearweight, leftweight, rightweight;
	unsigned int	frontR, frontG, frontB, rearR, rearG, rearB, leftR, leftG, leftB, rightR, rightG, rightB;
	unsigned int	frontsrcx, frontsrcy;
	unsigned int	rearsrcx, rearsrcy;
	unsigned int	leftsrcx, leftsrcy;
	unsigned int	rightsrcx, rightsrcy;
	unsigned char	*dst; 
	int xend = xoffset+STITCH_IMAGE_WIDTH/NX;
	int yend = yoffset+STITCH_IMAGE_HEIGHT/NY;
	int srcImageWidth = IMAGE_WIDTH*ACCURACY_NUM*2;
	int srcImageHeight = IMAGE_HEIGHT*ACCURACY_NUM*2;
	int halfwidth = srcImageWidth/2;
	int halfheight = srcImageHeight/2;
	for (int y = yoffset; y < yend; ++y)
	{
		dst = stitchImage+(y*dstwidthstep+xoffset)*4;
		for (int x = xoffset; x < xend; ++x)
		{
			frontweight = front[y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH*2+x];
			rearweight	= rear[y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH*2+x];
			leftweight	= left[y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH*2+x];
			rightweight = right[y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH*2+x];
			//front
			if (frontweight < 1)
			{
				frontR = 0;
				frontG = 0;
				frontB = 0;
			}else
			{
				frontsrcx = front[y*STITCH_IMAGE_WIDTH*3+x];
				frontsrcy = front[y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH+x];
				if (frontsrcx < halfwidth && frontsrcy < halfheight)
				{
					frontB = srcImage[(frontsrcy*2880+frontsrcx)<<2];
					frontG = srcImage[((frontsrcy*2880+frontsrcx)<<2)+1];					
					frontR = srcImage[((frontsrcy*2880+frontsrcx)<<2)+2];
				}else
				{
					frontR = 0;
					frontG = 0;
					frontB = 0;
				}
			}
			//rear
			if (rearweight < 1)
			{
				rearR = 0;
				rearG = 0;
				rearB = 0;
			}else
			{
				rearsrcx = rear[y*STITCH_IMAGE_WIDTH*3+x];
				rearsrcy = rear[y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH+x];
				if (rearsrcx > halfwidth && rearsrcx < srcImageWidth && rearsrcy < srcImageHeight)
				{
					rearB = srcImage[(rearsrcy*2880+rearsrcx)<<2];
					rearG = srcImage[((rearsrcy*2880+rearsrcx)<<2)+1];					
					rearR = srcImage[((rearsrcy*2880+rearsrcx)<<2)+2];
				}else
				{
					rearR = 0;
					rearG = 0;
					rearB = 0;
				}

			}
			//left
			if (leftweight < 1)
			{
				leftR = 0;
				leftG = 0;
				leftB = 0;
			}else
			{
				leftsrcx = left[y*STITCH_IMAGE_WIDTH*3+x];
				leftsrcy = left[y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH+x];
				if (leftsrcx < halfwidth && leftsrcy > halfheight && leftsrcy < srcImageHeight)
				{
					leftB = srcImage[(leftsrcy*2880+leftsrcx)<<2];
					leftG = srcImage[((leftsrcy*2880+leftsrcx)<<2)+1];					
					leftR = srcImage[((leftsrcy*2880+leftsrcx)<<2)+2];
				}else
				{
					leftR = 0;
					leftG = 0;
					leftB = 0;
				}
			}
			//right
			if (rightweight < 1)
			{
				rightR = 0;
				rightG = 0;
				rightB = 0;
			}else
			{
				rightsrcx = right[y*STITCH_IMAGE_WIDTH*3+x];
				rightsrcy = right[y*STITCH_IMAGE_WIDTH*3+STITCH_IMAGE_WIDTH+x];
				if (rightsrcx > halfwidth && rightsrcx < srcImageWidth && 
					rightsrcy > halfheight && rightsrcy < srcImageHeight)
				{
					rightB = srcImage[(rightsrcy*2880+rightsrcx)<<2];
					rightG = srcImage[((rightsrcy*2880+rightsrcx)<<2)+1];					
					rightR = srcImage[((rightsrcy*2880+rightsrcx)<<2)+2];
				}else
				{
					rightR = 0;
					rightG = 0;
					rightB = 0;
				}
			}
			//dstval
			*(dst)		= (frontweight*frontB+rearweight*rearB+leftweight*leftB+rightweight*rightB)>>WeightAccuracyBit;
			*(dst+1)	= (frontweight*frontG+rearweight*rearG+leftweight*leftG+rightweight*rightG)>>WeightAccuracyBit;
			*(dst+2)	= (frontweight*frontR+rearweight*rearR+leftweight*leftR+rightweight*rightR)>>WeightAccuracyBit;
			*(dst+3)	= 0;
			dst+=4;
		}
	}

	return true;
}

/************************************************************************/
/*				多线程调用(定点运算、不做插值、单幅源图像做处理)		*/
/*              根据全景表生成全景图                                    */
/*				unsigned int *frontcoordinatexMat: 前向X坐标表			*/
/*				unsigned int *frontcoordinateyMat: 前向Y坐标表			*/
/*				unsigned int *frontweightMat:		前向权值表			*/
/*				unsigned int *rearcoordinatexMat:	后向X坐标表			*/
/*				unsigned int *rearcoordinateyMat:	后向Y坐标表			*/
/*				unsigned int *rearweightMat:		后向权值表			*/
/*				unsigned int *leftcoordinatexMat:	左向X坐标表			*/
/*				unsigned int *leftcoordinateyMat:	左向Y坐标表			*/
/*				unsigned int *leftweightMat:		左向权值表			*/
/*				unsigned int *rightcoordinatexMat: 右向X坐标表			*/
/*				unsigned int *rightcoordinateyMat: 右向Y坐标表			*/
/*				unsigned char *frontImage	前向源图像内存地址			*/
/*				unsigned char *rearImage	后向源图像内存地址			*/
/*				unsigned char *leftImage	左向源图像内存地址			*/
/*				unsigned char *rightImage	右向源图像内存地址			*/
/*				unsigned char *stitchImage	输出全景图像内存地址		*/
/*				int xoffset:				x坐标偏移量					*/
/*				int yoffset:				y坐标偏移量					*/
/*				int dstwidthstep:			显示到显示屏的宽			*/
/*				int NX:						水平方向被分成NX部分		*/
/*				int NY:						垂直方向被分成NY部分		*/
/************************************************************************/
bool GetStitchImage6(unsigned int *frontcoordinatexMat, unsigned int *frontcoordinateyMat, unsigned int *frontweightMat, unsigned int *rearcoordinatexMat, 
	unsigned int *rearcoordinateyMat, unsigned int *rearweightMat, unsigned int *leftcoordinatexMat, unsigned int *leftcoordinateyMat, unsigned int *leftweightMat, 
	unsigned int *rightcoordinatexMat, unsigned int *rightcoordinateyMat, unsigned int *rightweightMat, unsigned char *srcImage, unsigned char *stitchImage, 
	int xoffset, int yoffset, int dstwidthstep, int NX, int NY)
{
	unsigned int	frontweight, rearweight, leftweight, rightweight;
	unsigned int	frontR, frontG, frontB, rearR, rearG, rearB, leftR, leftG, leftB, rightR, rightG, rightB;
	unsigned int	frontsrcx, frontsrcy;
	unsigned int	rearsrcx, rearsrcy;
	unsigned int	leftsrcx, leftsrcy;
	unsigned int	rightsrcx, rightsrcy;
	unsigned char	*dst; 
	int xend = xoffset+STITCH_IMAGE_WIDTH/NX;
	int yend = yoffset+STITCH_IMAGE_HEIGHT/NY;
	int srcImageWidth = IMAGE_WIDTH*ACCURACY_NUM*2;
	int srcImageHeight = IMAGE_HEIGHT*ACCURACY_NUM*2;
	int halfwidth = srcImageWidth/2;
	int halfheight = srcImageHeight/2;
	for (int y = yoffset; y < yend; ++y)
	{
		dst = stitchImage+(y*dstwidthstep+xoffset)*4;
		for (int x = xoffset; x < xend; ++x)
		{
			frontweight = frontweightMat[y*STITCH_IMAGE_WIDTH+x];
			rearweight	= rearweightMat[y*STITCH_IMAGE_WIDTH+x];
			leftweight	= leftweightMat[y*STITCH_IMAGE_WIDTH+x];
			rightweight = rightweightMat[y*STITCH_IMAGE_WIDTH+x];
			//front
			if (frontweight < 1)
			{
				frontR = 0;
				frontG = 0;
				frontB = 0;
			}else
			{
				frontsrcx = frontcoordinatexMat[y*STITCH_IMAGE_WIDTH+x];
				frontsrcy = frontcoordinateyMat[y*STITCH_IMAGE_WIDTH+x];
				if (frontsrcx < halfwidth && frontsrcy < halfheight)
				{
					frontB = srcImage[(frontsrcy*2880+frontsrcx)<<2];
					frontG = srcImage[((frontsrcy*2880+frontsrcx)<<2)+1];					
					frontR = srcImage[((frontsrcy*2880+frontsrcx)<<2)+2];
				}else
				{
					frontR = 0;
					frontG = 0;
					frontB = 0;
				}
			}
			//rear
			if (rearweight < 1)
			{
				rearR = 0;
				rearG = 0;
				rearB = 0;
			}else
			{
				rearsrcx = rearcoordinatexMat[y*STITCH_IMAGE_WIDTH+x];
				rearsrcy = rearcoordinateyMat[y*STITCH_IMAGE_WIDTH+x];
				if (rearsrcx > halfwidth && rearsrcx < srcImageWidth && rearsrcy < srcImageHeight)
				{
					rearB = srcImage[(rearsrcy*2880+rearsrcx)<<2];
					rearG = srcImage[((rearsrcy*2880+rearsrcx)<<2)+1];					
					rearR = srcImage[((rearsrcy*2880+rearsrcx)<<2)+2];
				}else
				{
					rearR = 0;
					rearG = 0;
					rearB = 0;
				}

			}
			//left
			if (leftweight < 1)
			{
				leftR = 0;
				leftG = 0;
				leftB = 0;
			}else
			{
				leftsrcx = leftcoordinatexMat[y*STITCH_IMAGE_WIDTH+x];
				leftsrcy = leftcoordinateyMat[y*STITCH_IMAGE_WIDTH+x];
				if (leftsrcx < halfwidth && leftsrcy > halfheight && leftsrcy < srcImageHeight)
				{
					leftB = srcImage[(leftsrcy*2880+leftsrcx)<<2];
					leftG = srcImage[((leftsrcy*2880+leftsrcx)<<2)+1];					
					leftR = srcImage[((leftsrcy*2880+leftsrcx)<<2)+2];
				}else
				{
					leftR = 0;
					leftG = 0;
					leftB = 0;
				}
			}
			//right
			if (rightweight < 1)
			{
				rightR = 0;
				rightG = 0;
				rightB = 0;
			}else
			{
				rightsrcx = rightcoordinatexMat[y*STITCH_IMAGE_WIDTH+x];
				rightsrcy = rightcoordinateyMat[y*STITCH_IMAGE_WIDTH+x];
				if (rightsrcx > halfwidth && rightsrcx < srcImageWidth && 
					rightsrcy > halfheight && rightsrcy < srcImageHeight)
				{
					rightB = srcImage[(rightsrcy*2880+rightsrcx)<<2];
					rightG = srcImage[((rightsrcy*2880+rightsrcx)<<2)+1];					
					rightR = srcImage[((rightsrcy*2880+rightsrcx)<<2)+2];
				}else
				{
					rightR = 0;
					rightG = 0;
					rightB = 0;
				}
			}
			//dstval
			*(dst)		= (frontweight*frontB+rearweight*rearB+leftweight*leftB+rightweight*rightB)>>WeightAccuracyBit;
			*(dst+1)	= (frontweight*frontG+rearweight*rearG+leftweight*leftG+rightweight*rightG)>>WeightAccuracyBit;
			*(dst+2)	= (frontweight*frontR+rearweight*rearR+leftweight*leftR+rightweight*rightR)>>WeightAccuracyBit;
			*(dst+3)	= 0;
			dst+=4;
		}
	}

	return true;
}
/************************************************************************/
/*				多线程调用(定点运算、单幅源图像做处理)					*/
/*              根据全景表生成全景图                                    */
/*				unsigned int *frontcoordinatexMat: 前向X坐标表			*/
/*				unsigned int *frontcoordinateyMat: 前向Y坐标表			*/
/*				unsigned int *frontweightMat:		前向权值表			*/
/*				unsigned int *rearcoordinatexMat:	后向X坐标表			*/
/*				unsigned int *rearcoordinateyMat:	后向Y坐标表			*/
/*				unsigned int *rearweightMat:		后向权值表			*/
/*				unsigned int *leftcoordinatexMat:	左向X坐标表			*/
/*				unsigned int *leftcoordinateyMat:	左向Y坐标表			*/
/*				unsigned int *leftweightMat:		左向权值表			*/
/*				unsigned int *rightcoordinatexMat: 右向X坐标表			*/
/*				unsigned int *rightcoordinateyMat: 右向Y坐标表			*/
/*				unsigned char *frontImage	前向源图像内存地址			*/
/*				unsigned char *rearImage	后向源图像内存地址			*/
/*				unsigned char *leftImage	左向源图像内存地址			*/
/*				unsigned char *rightImage	右向源图像内存地址			*/
/*				unsigned char *stitchImage	输出全景图像内存地址		*/
/*				int xoffset:				x坐标偏移量					*/
/*				int yoffset:				y坐标偏移量					*/
/*				int dstwidthstep:			显示到显示屏的宽			*/
/*				int NX:						水平方向被分成NX部分		*/
/*				int NY:						垂直方向被分成NY部分		*/
/************************************************************************/
bool GetStitchImage7(unsigned int *frontcoordinatexMat, unsigned int *frontcoordinateyMat, unsigned int *frontweightMat, unsigned int *rearcoordinatexMat, 
	unsigned int *rearcoordinateyMat, unsigned int *rearweightMat, unsigned int *leftcoordinatexMat, unsigned int *leftcoordinateyMat, unsigned int *leftweightMat, 
	unsigned int *rightcoordinatexMat, unsigned int *rightcoordinateyMat, unsigned int *rightweightMat, unsigned char *srcImage, unsigned char *stitchImage, 
	int xoffset, int yoffset, int dstwidthstep, int NX, int NY)
{
	unsigned int	frontweight, rearweight, leftweight, rightweight;
	unsigned int	frontR, frontG, frontB, rearR, rearG, rearB, leftR, leftG, leftB, rightR, rightG, rightB;
	unsigned int	fronttmpvalR1, fronttmpvalR2, fronttmpvalR3, fronttmpvalR4;
	unsigned int	fronttmpvalG1, fronttmpvalG2, fronttmpvalG3, fronttmpvalG4;
	unsigned int	fronttmpvalB1, fronttmpvalB2, fronttmpvalB3, fronttmpvalB4;
	unsigned int	reartmpvalR1, reartmpvalR2, reartmpvalR3, reartmpvalR4;
	unsigned int	reartmpvalG1, reartmpvalG2, reartmpvalG3, reartmpvalG4;
	unsigned int	reartmpvalB1, reartmpvalB2, reartmpvalB3, reartmpvalB4;
	unsigned int	lefttmpvalR1, lefttmpvalR2, lefttmpvalR3, lefttmpvalR4;
	unsigned int	lefttmpvalG1, lefttmpvalG2, lefttmpvalG3, lefttmpvalG4;
	unsigned int	lefttmpvalB1, lefttmpvalB2, lefttmpvalB3, lefttmpvalB4;
	unsigned int	righttmpvalR1, righttmpvalR2, righttmpvalR3, righttmpvalR4;
	unsigned int	righttmpvalG1, righttmpvalG2, righttmpvalG3, righttmpvalG4;
	unsigned int	righttmpvalB1, righttmpvalB2, righttmpvalB3, righttmpvalB4;
	unsigned int	frontsrcx, frontsrcy, frontpx, frontpy;
	unsigned int	rearsrcx, rearsrcy, rearpx, rearpy;
	unsigned int	leftsrcx, leftsrcy, leftpx, leftpy;
	unsigned int	rightsrcx, rightsrcy, rightpx, rightpy;
	unsigned char	*dst; 
	unsigned int	tmpfrontx, tmpfronty, tmprearx, tmpreary, tmpleftx, tmplefty, tmprightx, tmprighty;
	int xend = xoffset+STITCH_IMAGE_WIDTH/NX;
	int yend = yoffset+STITCH_IMAGE_HEIGHT/NY;
	int srcImageWidth = 1440;
	int srcImageHeight = 960;
	int halfwidth = srcImageWidth/2;
	int halfheight = srcImageHeight/2;
	for (int y = yoffset; y < yend; ++y)
	{
		dst = stitchImage+(y*dstwidthstep+xoffset)*4;
		for (int x = xoffset; x < xend; ++x)
		{
			frontweight = frontweightMat[y*STITCH_IMAGE_WIDTH+x];
			rearweight	= rearweightMat[y*STITCH_IMAGE_WIDTH+x];
			leftweight	= leftweightMat[y*STITCH_IMAGE_WIDTH+x];
			rightweight = rightweightMat[y*STITCH_IMAGE_WIDTH+x];
			//front
			if (frontweight < 1)
			{
				frontR = 0;
				frontG = 0;
				frontB = 0;
			}else
			{
				tmpfrontx = frontcoordinatexMat[y*STITCH_IMAGE_WIDTH+x];
				tmpfronty = frontcoordinateyMat[y*STITCH_IMAGE_WIDTH+x];
				frontsrcx = tmpfrontx >> ACCURACY_BIT;
				frontsrcy = tmpfronty >> ACCURACY_BIT;
				if (frontsrcx < halfwidth && frontsrcy < halfheight)
				{
					fronttmpvalB1 = srcImage[(frontsrcy*1440+frontsrcx)*4];
					fronttmpvalB2 = srcImage[(frontsrcy*1440+frontsrcx+1)*4];
					fronttmpvalB3 = srcImage[((frontsrcy+1)*1440+frontsrcx)*4];
					fronttmpvalB4 = srcImage[((frontsrcy+1)*1440+frontsrcx+1)*4];
					fronttmpvalG1 = srcImage[(frontsrcy*1440+frontsrcx)*4+1];
					fronttmpvalG2 = srcImage[(frontsrcy*1440+frontsrcx+1)*4+1];
					fronttmpvalG3 = srcImage[((frontsrcy+1)*1440+frontsrcx)*4+1];
					fronttmpvalG4 = srcImage[((frontsrcy+1)*1440+frontsrcx+1)*4+1];
					fronttmpvalR1 = srcImage[(frontsrcy*1440+frontsrcx)*4+2];
					fronttmpvalR2 = srcImage[(frontsrcy*1440+frontsrcx+1)*4+2];
					fronttmpvalR3 = srcImage[((frontsrcy+1)*1440+frontsrcx)*4+2];
					fronttmpvalR4 = srcImage[((frontsrcy+1)*1440+frontsrcx+1)*4+2];

					frontpx = tmpfrontx&ACCURACY_POINT;
					frontpy = tmpfronty&ACCURACY_POINT;
					frontR = ((ACCURACY_NUM-frontpx)*(ACCURACY_NUM-frontpy)*fronttmpvalR1+frontpx*(ACCURACY_NUM-frontpy)*fronttmpvalR2+
						(ACCURACY_NUM-frontpx)*frontpy*fronttmpvalR3+frontpx*frontpy*fronttmpvalR4)>>(ACCURACY_BIT<<1);
					frontG = ((ACCURACY_NUM-frontpx)*(ACCURACY_NUM-frontpy)*fronttmpvalG1+frontpx*(ACCURACY_NUM-frontpy)*fronttmpvalG2+
						(ACCURACY_NUM-frontpx)*frontpy*fronttmpvalG3+frontpx*frontpy*fronttmpvalG4)>>(ACCURACY_BIT<<1);
					frontB = ((ACCURACY_NUM-frontpx)*(ACCURACY_NUM-frontpy)*fronttmpvalB1+frontpx*(ACCURACY_NUM-frontpy)*fronttmpvalB2+
						(ACCURACY_NUM-frontpx)*frontpy*fronttmpvalB3+frontpx*frontpy*fronttmpvalB4)>>(ACCURACY_BIT<<1);
				}else
				{
					frontR = 0;
					frontG = 0;
					frontB = 0;
				}
			}
			//rear
			if (rearweight < 1)
			{
				rearR = 0;
				rearG = 0;
				rearB = 0;
			}else
			{
				tmpfrontx = rearcoordinatexMat[y*STITCH_IMAGE_WIDTH+x];
				tmpfronty = rearcoordinateyMat[y*STITCH_IMAGE_WIDTH+x];
				rearsrcx = tmpfrontx >> ACCURACY_BIT;
				rearsrcy = tmpfronty >> ACCURACY_BIT;
				if (rearsrcx > halfwidth && rearsrcx < srcImageWidth && rearsrcy < srcImageHeight)
				{
					reartmpvalB1 = srcImage[(rearsrcy*1440+rearsrcx)*4];
					reartmpvalB2 = srcImage[(rearsrcy*1440+rearsrcx+1)*4];
					reartmpvalB3 = srcImage[((rearsrcy+1)*1440+rearsrcx)*4];
					reartmpvalB4 = srcImage[((rearsrcy+1)*1440+rearsrcx+1)*4];
					reartmpvalG1 = srcImage[(rearsrcy*1440+rearsrcx)*4+1];
					reartmpvalG2 = srcImage[(rearsrcy*1440+rearsrcx+1)*4+1];
					reartmpvalG3 = srcImage[((rearsrcy+1)*1440+rearsrcx)*4+1];
					reartmpvalG4 = srcImage[((rearsrcy+1)*1440+rearsrcx+1)*4+1];
					reartmpvalR1 = srcImage[(rearsrcy*1440+rearsrcx)*4+2];
					reartmpvalR2 = srcImage[(rearsrcy*1440+rearsrcx+1)*4+2];
					reartmpvalR3 = srcImage[((rearsrcy+1)*1440+rearsrcx)*4+2];
					reartmpvalR4 = srcImage[((rearsrcy+1)*1440+rearsrcx+1)*4+2];

					rearpx = tmpfrontx&ACCURACY_POINT;
					rearpy = tmpfronty&ACCURACY_POINT;
					rearR = ((ACCURACY_NUM-rearpx)*(ACCURACY_NUM-rearpy)*reartmpvalR1+rearpx*(ACCURACY_NUM-rearpy)*reartmpvalR2+
						(ACCURACY_NUM-rearpx)*rearpy*reartmpvalR3+rearpx*rearpy*reartmpvalR4)>>(ACCURACY_BIT<<1);
					rearG = ((ACCURACY_NUM-rearpx)*(ACCURACY_NUM-rearpy)*reartmpvalG1+rearpx*(ACCURACY_NUM-rearpy)*reartmpvalG2+
						(ACCURACY_NUM-rearpx)*rearpy*reartmpvalG3+rearpx*rearpy*reartmpvalG4)>>(ACCURACY_BIT<<1);
					rearB = ((ACCURACY_NUM-rearpx)*(ACCURACY_NUM-rearpy)*reartmpvalB1+rearpx*(ACCURACY_NUM-rearpy)*reartmpvalB2+
						(ACCURACY_NUM-rearpx)*rearpy*reartmpvalB3+rearpx*rearpy*reartmpvalB4)>>(ACCURACY_BIT<<1);
				}else
				{
					rearR = 0;
					rearG = 0;
					rearB = 0;
				}

			}
			//left
			if (leftweight < 1)
			{
				leftR = 0;
				leftG = 0;
				leftB = 0;
			}else
			{
				tmpfrontx = leftcoordinatexMat[y*STITCH_IMAGE_WIDTH+x];
				tmpfronty = leftcoordinateyMat[y*STITCH_IMAGE_WIDTH+x];
				leftsrcx = tmpfrontx >> ACCURACY_BIT;
				leftsrcy = tmpfronty >> ACCURACY_BIT;
				if (leftsrcx < halfwidth && leftsrcy > halfheight && leftsrcy < srcImageHeight)
				{
					lefttmpvalB1 = srcImage[(leftsrcy*1440+leftsrcx)*4];
					lefttmpvalB2 = srcImage[(leftsrcy*1440+leftsrcx+1)*4];
					lefttmpvalB3 = srcImage[((leftsrcy+1)*1440+leftsrcx)*4];
					lefttmpvalB4 = srcImage[((leftsrcy+1)*1440+leftsrcx+1)*4];
					lefttmpvalG1 = srcImage[(leftsrcy*1440+leftsrcx)*4+1];
					lefttmpvalG2 = srcImage[(leftsrcy*1440+leftsrcx+1)*4+1];
					lefttmpvalG3 = srcImage[((leftsrcy+1)*1440+leftsrcx)*4+1];
					lefttmpvalG4 = srcImage[((leftsrcy+1)*1440+leftsrcx+1)*4+1];
					lefttmpvalR1 = srcImage[(leftsrcy*1440+leftsrcx)*4+2];
					lefttmpvalR2 = srcImage[(leftsrcy*1440+leftsrcx+1)*4+2];
					lefttmpvalR3 = srcImage[((leftsrcy+1)*1440+leftsrcx)*4+2];
					lefttmpvalR4 = srcImage[((leftsrcy+1)*1440+leftsrcx+1)*4+2];

					leftpx = tmpfrontx&ACCURACY_POINT;
					leftpy = tmpfronty&ACCURACY_POINT;
					leftR = ((ACCURACY_NUM-leftpx)*(ACCURACY_NUM-leftpy)*lefttmpvalR1+leftpx*(ACCURACY_NUM-leftpy)*lefttmpvalR2+
						(ACCURACY_NUM-leftpx)*leftpy*lefttmpvalR3+leftpx*leftpy*lefttmpvalR4)>>(ACCURACY_BIT<<1);
					leftG = ((ACCURACY_NUM-leftpx)*(ACCURACY_NUM-leftpy)*lefttmpvalG1+leftpx*(ACCURACY_NUM-leftpy)*lefttmpvalG2+
						(ACCURACY_NUM-leftpx)*leftpy*lefttmpvalG3+leftpx*leftpy*lefttmpvalG4)>>(ACCURACY_BIT<<1);
					leftB = ((ACCURACY_NUM-leftpx)*(ACCURACY_NUM-leftpy)*lefttmpvalB1+leftpx*(ACCURACY_NUM-leftpy)*lefttmpvalB2+
						(ACCURACY_NUM-leftpx)*leftpy*lefttmpvalB3+leftpx*leftpy*lefttmpvalB4)>>(ACCURACY_BIT<<1);
				}else
				{
					leftR = 0;
					leftG = 0;
					leftB = 0;
				}

			}
			//right
			if (rightweight < 1)
			{
				rightR = 0;
				rightG = 0;
				rightB = 0;
			}else
			{
				tmpfrontx = rightcoordinatexMat[y*STITCH_IMAGE_WIDTH+x];
				tmpfronty = rightcoordinateyMat[y*STITCH_IMAGE_WIDTH+x];
				rightsrcx = tmpfrontx >> ACCURACY_BIT;
				rightsrcy = tmpfronty >> ACCURACY_BIT;
				if (rightsrcx > halfwidth && rightsrcx < srcImageWidth && 
					rightsrcy > halfheight && rightsrcy < srcImageHeight)
				{
					righttmpvalB1 = srcImage[(rightsrcy*1440+rightsrcx)*4];
					righttmpvalB2 = srcImage[(rightsrcy*1440+rightsrcx+1)*4];
					righttmpvalB3 = srcImage[((rightsrcy+1)*1440+rightsrcx)*4];
					righttmpvalB4 = srcImage[((rightsrcy+1)*1440+rightsrcx+1)*4];
					righttmpvalG1 = srcImage[(rightsrcy*1440+rightsrcx)*4+1];
					righttmpvalG2 = srcImage[(rightsrcy*1440+rightsrcx+1)*4+1];
					righttmpvalG3 = srcImage[((rightsrcy+1)*1440+rightsrcx)*4+1];
					righttmpvalG4 = srcImage[((rightsrcy+1)*1440+rightsrcx+1)*4+1];
					righttmpvalR1 = srcImage[(rightsrcy*1440+rightsrcx)*4+2];
					righttmpvalR2 = srcImage[(rightsrcy*1440+rightsrcx+1)*4+2];
					righttmpvalR3 = srcImage[((rightsrcy+1)*1440+rightsrcx)*4+2];
					righttmpvalR4 = srcImage[((rightsrcy+1)*1440+rightsrcx+1)*4+2];

					rightpx = tmpfrontx&ACCURACY_POINT;
					rightpy = tmpfronty&ACCURACY_POINT;
					rightR = ((ACCURACY_NUM-rightpx)*(ACCURACY_NUM-rightpy)*righttmpvalR1+rightpx*(ACCURACY_NUM-rightpy)*righttmpvalR2+
						(ACCURACY_NUM-rightpx)*rightpy*righttmpvalR3+rightpx*rightpy*righttmpvalR4)>>(ACCURACY_BIT<<1);
					rightG = ((ACCURACY_NUM-rightpx)*(ACCURACY_NUM-rightpy)*righttmpvalG1+rightpx*(ACCURACY_NUM-rightpy)*righttmpvalG2+
						(ACCURACY_NUM-rightpx)*rightpy*righttmpvalG3+rightpx*rightpy*righttmpvalG4)>>(ACCURACY_BIT<<1);
					rightB = ((ACCURACY_NUM-rightpx)*(ACCURACY_NUM-rightpy)*righttmpvalB1+rightpx*(ACCURACY_NUM-rightpy)*righttmpvalB2+
						(ACCURACY_NUM-rightpx)*rightpy*righttmpvalB3+rightpx*rightpy*righttmpvalB4)>>(ACCURACY_BIT<<1);
				}else
				{
					rightR = 0;
					rightG = 0;
					rightB = 0;
				}
			}
			//dstval
			*(dst)		= (frontweight*frontB+rearweight*rearB+leftweight*leftB+rightweight*rightB)>>WeightAccuracyBit;
			*(dst+1)	= (frontweight*frontG+rearweight*rearG+leftweight*leftG+rightweight*rightG)>>WeightAccuracyBit;
			*(dst+2)	= (frontweight*frontR+rearweight*rearR+leftweight*leftR+rightweight*rightR)>>WeightAccuracyBit;
			*(dst+3)	= 0;
			dst+=4;
		}
	}

	return true;
}
/************************************************************************/
/*				融合                                                    */
/************************************************************************/
bool BlendImage(StitchInfo *front, StitchInfo *rear, StitchInfo *left, StitchInfo *right)
{
	if (!front || !rear || !left || !right)
	{
		return false;
	}
	char *dst = front->stitchImage->imageData;
	char *srcFront = front->transformImage->imageData;
	char *srcRear = rear->transformImage->imageData;
	char *srcLeft = left->transformImage->imageData;
	char *srcRight = right->transformImage->imageData;
	float A1, B1, C1, AABB1, D1;
	float A2, B2, C2, AABB2, D2;

#define MaxBlend 5

	for (int y = 0; y < front->stitchImage->height; ++y)
	{
		for (int x = 0; x < front->stitchImage->width; ++x)
		{
			if (JudgeIsInside(cvPoint(x, y), front->stitchPoint, 6))
			{
				//前向
				A1 = front->stitchPoint[5].y-front->stitchPoint[0].y;
				B1 = front->stitchPoint[0].x-front->stitchPoint[5].x;
				C1 = front->stitchPoint[5].x*front->stitchPoint[0].y-front->stitchPoint[0].x*front->stitchPoint[5].y;
				AABB1 = sqrt(A1*A1+B1*B1);
				D1 = abs(A1*x+B1*y+C1)/AABB1;
				A2 = front->stitchPoint[4].y-front->stitchPoint[3].y;
				B2 = front->stitchPoint[3].x-front->stitchPoint[4].x;
				C2 = front->stitchPoint[4].x*front->stitchPoint[3].y-front->stitchPoint[3].x*front->stitchPoint[4].y;
				AABB2 = sqrt(A2*A2+B2*B2);
				D2 = abs(A2*x+B2*y+C2)/AABB2;

				if (D1 < MaxBlend)
				{
					*dst = *(srcFront+(y*front->stitchImage->width+x)*4)*(D1/MaxBlend/2+0.5)+
							*(srcLeft+(y*front->stitchImage->width+x)*4)*(0.5-D1/MaxBlend/2);
					*(dst+1) = *(srcFront+(y*front->stitchImage->width+x)*4+1)*(D1/MaxBlend/2+0.5)+
								*(srcLeft+(y*front->stitchImage->width+x)*4+1)*(0.5-D1/MaxBlend/2);
					*(dst+2) = *(srcFront+(y*front->stitchImage->width+x)*4+2)*(D1/MaxBlend/2+0.5)+
								*(srcLeft+(y*front->stitchImage->width+x)*4+2)*(0.5-D1/MaxBlend/2);
					*(dst+3) = 0;
				}else if (D2 < MaxBlend)
				{
					*dst = *(srcFront+(y*front->stitchImage->width+x)*4) * (D2/MaxBlend/2+0.5)+
						*(srcRight+(y*front->stitchImage->width+x)*4) * (0.5-D2/MaxBlend/2);
					*(dst+1) = *(srcFront+(y*front->stitchImage->width+x)*4+1)*(D2/MaxBlend/2+0.5)+
						*(srcRight+(y*front->stitchImage->width+x)*4+1) * (0.5-D2/MaxBlend/2);
					*(dst+2) = *(srcFront+(y*front->stitchImage->width+x)*4+2) * (D2/MaxBlend/2+0.5)+
						*(srcRight+(y*front->stitchImage->width+x)*4+2) * (0.5-D2/MaxBlend/2);
					*(dst+3) = 0;
				}else
				{
					*dst = *(srcFront+(y*front->stitchImage->width+x)*4);
					*(dst+1) = *(srcFront+(y*front->stitchImage->width+x)*4+1);
					*(dst+2) = *(srcFront+(y*front->stitchImage->width+x)*4+2);
					*(dst+3) = 0;
				}				

			}else if (JudgeIsInside(cvPoint(x, y), rear->stitchPoint, 6))
			{
				//后向
				A1 = rear->stitchPoint[4].y-rear->stitchPoint[3].y;
				B1 = rear->stitchPoint[3].x-rear->stitchPoint[4].x;
				C1 = rear->stitchPoint[4].x*rear->stitchPoint[3].y-rear->stitchPoint[4].x*rear->stitchPoint[3].y;
				AABB1 = sqrt(A1*A1+B1*B1);
				D1 = abs(A1*x+B1*y+C1)/AABB1;
				A2 = rear->stitchPoint[5].y-rear->stitchPoint[0].y;
				B2 = rear->stitchPoint[0].x-rear->stitchPoint[5].x;
				C2 = rear->stitchPoint[5].x*rear->stitchPoint[0].y-rear->stitchPoint[0].x*rear->stitchPoint[5].y;
				AABB2 = sqrt(A2*A2+B2*B2);
				D2 = abs(A2*x+B2*y+C2)/AABB2;

				if (D1 < MaxBlend)
				{
					*dst = *(srcRear+(y*front->stitchImage->width+x)*4)*(D1/MaxBlend/2+0.5)+
						*(srcLeft+(y*front->stitchImage->width+x)*4)*(0.5-D1/MaxBlend/2);
					*(dst+1) = *(srcRear+(y*front->stitchImage->width+x)*4+1)*(D1/MaxBlend/2+0.5)+
						*(srcLeft+(y*front->stitchImage->width+x)*4+1)*(0.5-D1/MaxBlend/2);
					*(dst+2) = *(srcRear+(y*front->stitchImage->width+x)*4+2)*(D1/MaxBlend/2+0.5)+
						*(srcLeft+(y*front->stitchImage->width+x)*4+2)*(0.5-D1/MaxBlend/2);
					*(dst+3) = 0;
				}else if (D2 < MaxBlend)
				{
					*dst = *(srcRear+(y*front->stitchImage->width+x)*4)*(D2/MaxBlend/2+0.5)+
						*(srcRight+(y*front->stitchImage->width+x)*4)*(0.5-D2/MaxBlend/2);
					*(dst+1) = *(srcRear+(y*front->stitchImage->width+x)*4+1)*(D2/MaxBlend/2+0.5)+
						*(srcRight+(y*front->stitchImage->width+x)*4+1)*(0.5-D2/MaxBlend/2);
					*(dst+2) = *(srcRear+(y*front->stitchImage->width+x)*4+2)*(D2/MaxBlend/2+0.5)+
						*(srcRight+(y*front->stitchImage->width+x)*4+2)*(0.5-D2/MaxBlend/2);
					*(dst+3) = 0;
				}else
				{
					*dst = *(srcRear+(y*front->stitchImage->width+x)*4);
					*(dst+1) = *(srcRear+(y*front->stitchImage->width+x)*4+1);
					*(dst+2) = *(srcRear+(y*front->stitchImage->width+x)*4+2);
					*(dst+3) = 0;
				}
			}else if (JudgeIsInside(cvPoint(x, y), left->stitchPoint, 6))
			{
				//左侧
				A1 = left->stitchPoint[4].y-left->stitchPoint[3].y;
				B1 = left->stitchPoint[3].x-left->stitchPoint[4].x;
				C1 = left->stitchPoint[4].x*left->stitchPoint[3].y-left->stitchPoint[3].x*left->stitchPoint[4].y;
				AABB1 = sqrt(A1*A1+B1*B1);
				D1 = abs(A1*x+B1*y+C1)/AABB1;
				A2 = left->stitchPoint[5].y-left->stitchPoint[0].y;
				B2 = left->stitchPoint[0].x-left->stitchPoint[5].x;
				C2 = left->stitchPoint[5].x*left->stitchPoint[0].y-left->stitchPoint[0].x*left->stitchPoint[5].y;
				AABB2 = sqrt(A2*A2+B2*B2);
				D2 = abs(A2*x+B2*y+C2)/AABB2;

				if (D1 < MaxBlend)
				{
					*dst = *(srcLeft+(y*front->stitchImage->width+x)*4)*(D1/MaxBlend/2+0.5)+
						*(srcFront+(y*front->stitchImage->width+x)*4)*(0.5-D1/MaxBlend/2);
					*(dst+1) = *(srcLeft+(y*front->stitchImage->width+x)*4+1)*(D1/MaxBlend/2+0.5)+
						*(srcFront+(y*front->stitchImage->width+x)*4+1)*(0.5-D1/MaxBlend/2);
					*(dst+2) = *(srcLeft+(y*front->stitchImage->width+x)*4+2)*(D1/MaxBlend/2+0.5)+
						*(srcFront+(y*front->stitchImage->width+x)*4+2)*(0.5-D1/MaxBlend/2);
					*(dst+3) = 0;
				}else if (D2 < MaxBlend)
				{
					*dst = *(srcLeft+(y*front->stitchImage->width+x)*4)*(D2/MaxBlend/2+0.5)+
						*(srcRear+(y*front->stitchImage->width+x)*4)*(0.5-D2/MaxBlend/2);
					*(dst+1) = *(srcLeft+(y*front->stitchImage->width+x)*4+1)*(D2/MaxBlend/2+0.5)+
						*(srcRear+(y*front->stitchImage->width+x)*4+1)*(0.5-D2/MaxBlend/2);
					*(dst+2) = *(srcLeft+(y*front->stitchImage->width+x)*4+2)*(D2/MaxBlend/2+0.5)+
						*(srcRear+(y*front->stitchImage->width+x)*4+2)*(0.5-D2/MaxBlend/2);
					*(dst+3) = 0;
				}else
				{
					*dst = *(srcLeft+(y*front->stitchImage->width+x)*4);
					*(dst+1) = *(srcLeft+(y*front->stitchImage->width+x)*4+1);
					*(dst+2) = *(srcLeft+(y*front->stitchImage->width+x)*4+2);
					*(dst+3) = 0;
				}
			}else if (JudgeIsInside(cvPoint(x, y), right->stitchPoint, 6))
			{
				//右侧
				A1 = right->stitchPoint[5].y-right->stitchPoint[0].y;
				B1 = right->stitchPoint[0].x-right->stitchPoint[5].x;
				C1 = right->stitchPoint[5].x*right->stitchPoint[0].y-right->stitchPoint[0].x*right->stitchPoint[5].y;
				AABB1 = sqrt(A1*A1+B1*B1);
				D1 = abs(A1*x+B1*y+C1)/AABB1;
				A2 = right->stitchPoint[4].y-right->stitchPoint[3].y;
				B2 = right->stitchPoint[3].x-right->stitchPoint[4].x;
				C2 = right->stitchPoint[4].x*right->stitchPoint[3].y-right->stitchPoint[3].x*right->stitchPoint[4].y;
				AABB2 = sqrt(A2*A2+B2*B2);
				D2 = abs(A2*x+B2*y+C2)/AABB2;

				if (D1 < MaxBlend)
				{
					*dst = *(srcRight+(y*front->stitchImage->width+x)*4)*(D1/MaxBlend/2+0.5)+
						*(srcFront+(y*front->stitchImage->width+x)*4)*(0.5-D1/MaxBlend/2);
					*(dst+1) = *(srcRight+(y*front->stitchImage->width+x)*4+1)*(D1/MaxBlend/2+0.5)+
						*(srcFront+(y*front->stitchImage->width+x)*4+1)*(0.5-D1/MaxBlend/2);
					*(dst+2) = *(srcRight+(y*front->stitchImage->width+x)*4+2)*(D1/MaxBlend/2+0.5)+
						*(srcFront+(y*front->stitchImage->width+x)*4+2)*(0.5-D1/MaxBlend/2);
					*(dst+3) = 0;
				}else if (D2 < MaxBlend)
				{
					*dst = *(srcRight+(y*front->stitchImage->width+x)*4)*(D2/MaxBlend/2+0.5)+
						*(srcRear+(y*front->stitchImage->width+x)*4)*(0.5-D2/MaxBlend/2);
					*(dst+1) = *(srcRight+(y*front->stitchImage->width+x)*4+1)*(D2/MaxBlend/2+0.5)+
						*(srcRear+(y*front->stitchImage->width+x)*4+1)*(0.5-D2/MaxBlend/2);
					*(dst+2) = *(srcRight+(y*front->stitchImage->width+x)*4+2)*(D2/MaxBlend/2+0.5)+
						*(srcRear+(y*front->stitchImage->width+x)*4+2)*(0.5-D2/MaxBlend/2);
					*(dst+3) = 0;
				}else
				{
					*dst = *(srcRight+(y*front->stitchImage->width+x)*4);
					*(dst+1) = *(srcRight+(y*front->stitchImage->width+x)*4+1);
					*(dst+2) = *(srcRight+(y*front->stitchImage->width+x)*4+2);
					*(dst+3) = 0;
				}
			}else
			{
				*dst = 0;
				*(dst+1) = 0;
				*(dst+2) = 0;
				*(dst+3) = 0;
			}
			dst += 4;
		}
	}

	return true;
}
