#ifndef OPENCVUTILS_H
#define OPENCVUTILS_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <locale.h>

#include "object_categorization/JBKUtils.h"
#include "object_categorization/ThreeDUtils.h"
#include "object_categorization/Math3d.h"


//IplImage* img=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
//((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 0]=111; // B
//((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 1]=112; // G
//((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 2]=113; // R
// oder macro
// 

namespace ipa_utils {
/// class Point3Dbl;
/// class to handle sequences of the range images
class DblMatrixList : public std::vector<DblMatrix>
{
public:
	DblMatrixList(){};
	~DblMatrixList(){};
	int Size(){return (int)this->size();};
	void PushBackMatrix(const DblMatrix& m){std::vector<DblMatrix>::push_back(m);}
	void PushBackIplImage(IplImage* Image);
	void ImportFromIplImageXML(std::string Name, bool Append=false);
	unsigned long Load(std::string PathName, std::string FileName, std::string InfoFileName);
	unsigned long Delete(std::string PathName, std::string FileName, std::string InfoFileName);
	unsigned long Save(std::string Name);

	/// Returns the height of the first DblMatrix element within the list.
	/// It is assumed, that all matrices within the list have similar dimensions.
	/// @return The height of the double matrix.
	int GetHeight(){return (*this)[0].GetHeight();};
	
	/// Returns the width of the first DblMatrix element within the list.
	/// It is assumed, that all matrices within the list have similar dimensions.
	/// @return The width of the double matrix.
	int GetWidth(){return (*this)[0].GetWidth();};

	/// Returns number of list elements.
	/// The number of list elements corresponds to the number of double matrices within the list.
	/// @return Number of list elements.
	int GetLength(){return (int)this->size();};

	/// Returns the value of double matrix k on position [k][j].
	/// @return The value of double matrix k on position [k][j].
	double GetValue(int i, int j, int k) {return (*this)[k][j][i];};

	/// Returns the minimum
	void MinMax(int k, double& Min, double& Max){return (*this)[k].MinMax(Min, Max);}
	void GetLine(int k, int j, DblVector& Line, int Step=1);
	void GetColumn(int k, int i, DblVector& Column, int Step=1);	
};

// function to convert a float/double image into eight bit
void ConvertToShowImage(IplImage* Source, IplImage* Dest);

// function to get the min and max values for a three layer images
void GetMinMax(IplImage* Image, Point3Dbl& Min, Point3Dbl& Max);

/// Get box sum (sum of all intensity values within a quadric area)
/// of a one channel integral image computed with cvIntegral().
/// @param i X-Coordinate of the center of the box.
/// @param j Y-Coordinate of the center of the box.
/// @param r Radius of the box (r = width/2).
double BoxSum(IplImage* SrcInt, int i, int j, int r);

CvScalar BoxSumCvScalar(IplImage* SrcInt, int i, int j, int r);

// functions for image conversion
inline double BoxSum2(IplImage* SrcInt, int le, int ri, int to, int bo) {return cvGetReal2D(SrcInt, bo+1, ri+1)-cvGetReal2D(SrcInt, to, ri+1)
														-cvGetReal2D(SrcInt, bo+1, le)+cvGetReal2D(SrcInt, to, le);}

/// are set to the value of the upper or lower boundary, repectively. The processed range values
/// are colored according to a HSV-color schema, where S and V are fixed.
/// @param Range The 'raw' range image.
/// @param Out The resulting image for displaying.
/// @param Min Lower bound for range data computes to Min*5000 (Default Min = 1).
/// @param Max Upper bound for range data computes to Max*30000 (Default Maxs = 1).
void RangeDisplayImageConversion(IplImage* Range, IplImage* Out, double* Min=NULL, double* Max=NULL);
void IntenDisplayImageConversion(IplImage* Intensity, IplImage* Out);

/// Creates a combination of range image and intensity image for displaying.
/// Range data is limited to values between 2000 and 60000. Values exceeding the limits 
/// are set to the value of the upper or lower boundary, repectively. The processed range values
/// are colored according to a HSV-color schema, where S is fixed. H is determined based on the
/// range value and V based on the intensity value.
/// @param Range The 'raw' range image.
/// @param Intensity The 'raw' intensiy image.
/// @param Out The resulting image for displaying.
void RangeDisplayImageConversion(IplImage* Range, IplImage* Intensity, IplImage* Out);

/// Manipulates the range image data for displaying.
/// The given picture is colored according to the RGB-color schema.
/// The magnitude of a pixel's x-value determines the R value, the magnitude of a
/// pixel's y-value determines the G value and the magnitude of a pixel's z-value
/// determines the B value.
/// @param CoordImg The image of interest with cartesian coordinates.
/// @param Out The resulting image for displaying.
/// @param Min The minimal (x,y,z)-values of the image
/// @param Max The maximal (x,y,z)-values of the image
void CoordDisplayImageConversion(IplImage* CoordImg, IplImage* Out, Point3Dbl* Min=NULL, Point3Dbl* Max=NULL);
void CoordDisplayImageConversionOnlyZSpectral(IplImage* CoordImg, IplImage* Out, double MinZ=0, double MaxZ=0.5*7500.0);
/// Creates a grayscale image out of a image with given cartesian coordinates.
/// The given cartesian depth value is transformed in a corresponding gray value. The gray is darker,
/// the closer an object is located relative to the camera.
/// @param CoordImg The image with cartesian coordinates.
/// @param Out The resulting grayscale image.
/// @param MinZ Minimal z-value that is considered for grayscaling.
/// @param MaxZ Maximal z-value that is considered for grayscaling.

void CoordDisplayImageConversionOnlyZGray(IplImage* CoordImg, IplImage* Out, double MinZ=0.0, double MaxZ=0.5*7500.0);

// save and load
void SaveImageAsMatrix(std::string FileName, IplImage* RangeImage);
void LoadImageAsMatrix(std::string FileName, IplImage* RangeImage);
void ShowPointsInImage(IplImage* Img, const std::vector<CvPoint>& LimbPoints);

void MinFilter(IplImage* Image);

int CopyRotatedImageFrame(IplImage* Input, IplImage* Output, CvPoint Center, double Angle, int Fill, double sx=0, double sy=0);
double InterpolRealValue(IplImage* Img, double xpos, double ypos);
CvScalar InterpolValue(IplImage* Img, double xpos, double ypos);

// function to load a float image sequence as DblMatrixList

// funtion to get the gradient of an image point
void GetGradient(IplImage* Image, int x, int y);
void GetGradientSobel(IplImage* Image, int x, int y, double& dx, double& dy);

/// Function to get a scale dependend gradient at a 3D position.
void GetScaleGradient(IplImage* IntImage, int x, int y, int r, double& Mag, double& Phi);
//void GetScaleGradientInterpolated(IplImage* IntImage, int x, int y, int r, double& Mag, double& Phi);
void GetScaleGradientIter(IplImage* IntImage, int x, int y, int r, double& Mag, double& Phi);
void GetScaleGradientSobel(IplImage* IntImage, int x, int y, int r, double& Mag, double& Phi);

/// Funtion to get the 3D surface normal in an image point.
/// @param Image The coordinate image.
/// @param x The x-coordinate of the point of interest.
/// @param y The y-coordinate of the point of interest.
/// @param k The range of the considered neighbourhood (x+k till x-k and y+k till y-k).
/// @param Res The unit normal vector.
int GetNormal(IplImage* Image, int x, int y, Point3Dbl& Res, int k=1);
void GetNormalSobel(IplImage* Image, int x, int y, Point3Dbl& Res);
//void GetNormal2(IplImage* Image, int x, int y, Point3Dbl& Res, int k=1);

void SetBackgroundNeutral(IplImage* Image, CvScalar MaskVal=CV_RGB(0,0,0));

void TransformCoordinateImage(IplImage* CoordImage, Frame& F, bool Invert);

/*
// function to get the center (minimum) of a range image
void GetGlobalCenter(IplImage* Image, int& IMin, int& JMin, double Cut=0.0);

// functions to load and save images collections

int LoadImages(std::string Name, std::string Path="",
			   IplImage** Range=NULL, IplImage** Intensity=NULL,
			   IplImage** Coords=NULL, IplImage** Color=NULL);

int SaveImages(std::string Name, std::string Path="",
			   IplImage* Range=NULL, IplImage* Intensity=NULL,
			   IplImage* Coords=NULL, IplImage* Color=NULL);
*/

class ImageTriple
{
public:
	ImageTriple(IplImage* SRXYZImg=NULL, IplImage* SRIntenImg=NULL, IplImage* m_ColorImg=NULL);
	~ImageTriple();
	void Release();
	void Delete(std::string Name);
	unsigned long Load(std::string Name);
	unsigned long Save(std::string Name);
	IplImage* m_SRXYZImg;
	IplImage* m_SRIntenImg;
	IplImage* m_ColorImg;
};

class ImageTripleList : public std::vector<ImageTriple>
{
	/// Push to list (makes copies).
	void PushBack(IplImage* SRXYZImg, IplImage* SRIntenImg, IplImage* ColorImg);

	/// Load.
	unsigned long Load(std::string FileName);

	/// Save.
	unsigned long Save(std::string FileName);

	/// Clear.
	void Clear();
};

void TransformPoint(double x_in, double y_in, double z_in, double& x_out, double& y_out, double& z_out, const CvMat* R, const CvMat* t);
void TransformPointInverse(double x_in, double y_in, double z_in, double& x_out, double& y_out, double& z_out, const CvMat* R, const CvMat* t);
void TransformPointRodrigues(double x_in, double y_in, double z_in, double& x_out, double& y_out, double& z_out, const CvMat* r_vec, const CvMat* t);
void TransformPointRodriguesInverse(double x_in, double y_in, double z_in, double& x_out, double& y_out, double& z_out, const CvMat* r_vec, const CvMat* t);

void GetCorrectedUV(double u, double v, double fx, double cx, double fy, double cy, double w, double h,
					double k0, double k1, double k2, double k3, double& u_dot, double& v_dot);

void PerspectiveProjection(double x_in, double y_in, double z_in, double& u_out, double& v_out, const CvMat* A, const CvMat* distCoeffs=0);
//void PerspectiveProjectionInverseUndistorted(int u_in, int v_in, double z_in, double& x_out, double& y_out, const CvMat* A);
//void GetZCoeffsMap(CvMat* zCoeffs, const CvMat* A); 
//void GetCalibratedXYZImageFromUndistortedRangeImage(IplImage* rangeImg, CvMat* zCoeffs, const CvMat* A, const CvMat* distCoeffs);
//void GetXYZImageFromRangeImage(IplImage* RangeImage,
//							   IplImage* AZ, IplImage* BZ, IplImage* CZ,
//							   IplImage* X, IplImage* Y, IplImage* xyzImage);

void UndistortRangerImages(IplImage* rangeImage, IplImage* intenImage, IplImage* mapX, IplImage* mapY);

void ForwardTrafoSR(int u, int v, int t, double& x, double& y, double& z, double f, double cx, double cy, double k, double l, double m); 
void GetXYZImageFromUndistortedRangeImage(IplImage* rangeImage, double f, double k, double l, double m, IplImage* xyzImage);

void ConvertIntTo3x8Bit(short x, unsigned char& low, unsigned char& high, unsigned char& sign);
void Convert3x8BitToInt(unsigned char low, unsigned char high, unsigned char sign, short& x);
//void Convert32FltTo3x8Bit(IplImage* fltImage, IplImage* eightBit3ChannelsImage);
//void Convert3x8BitTo32Flt(IplImage* fltImage, IplImage* eightBit3ChannelsImage);

void ConvertFrame(const Mat3d& R, const Vec3d& t, ipa_utils::Frame& F);
void ConvertFrameBack(const ipa_utils::Frame& F, Mat3d& R, Vec3d& t);

} // end namespace ipa_utils


#endif
