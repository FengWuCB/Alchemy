#ifndef _KREADER_H
#define _KREADER_H

/*
***************************************************************************************************************
* Kinect �Ӵ򿪵���ȡ���ݵ�����
*
*                                         /- Depth:get_DepthFrameSource()->OpenReader()->AcquireLatestFrame()
*                                         |
* Kinect:GetDefaultKinectSensor()->Open()-|- Color:get_ColorFrameSource()->OpenReader()->AcquireLatestFrame()
*                                         |
*                                         |- IR:get_InfraredFrameSource()->OpenReader()->AcquireLatestFrame()
*                                         \....
*
* Kinect �ṩ�ˡ��¼�����(event)�͡���ѯ��(polling)���ֲ���ģʽ
***************************************************************************************************************
*/

#include "zcore\config.h"

#ifdef USING_KINECT
#include <Kinect.h>
#include <opencv2\core.hpp>



typedef enum _FrameTypes
{
	FrameTypes_None = 0x00,
	FrameTypes_Color = 0x01,
	FrameTypes_Infra = 0x02,
	FrameTypes_Depth = 0x04,
	FrameTypes_All = 0x07
} FrameTypes;


/**
 * @KinectSensor class
 * @berif ��װKinect������Ϊ�࣬��ȡ��Kinect�л�ȡ�ĸ������ݣ�����openCV��Mat�����ʽ����
 */
class KinectSensor {
public:
	KinectSensor();
	KinectSensor(FrameTypes _type);
	KinectSensor(const KinectSensor &) = delete;                                                  // ��ֹ����
	KinectSensor &operator=(const KinectSensor &) = delete;                                       // ��ֹ��ֵ
	~KinectSensor();

	//Check for new frame  
	HRESULT isNewFrameArrived(FrameTypes frameTypes);

	// Update data
	HRESULT update(FrameTypes _type);

	// ��ȡ��openCV��Mat�ౣ���ͼ������
	inline cv::Mat getColorImg() { return colorImg; }
	inline cv::Mat getDepthImg() { return depthImg; }
	inline cv::Mat getInfraImg() { return infraImg; }

	// Get time stamp
	inline INT64 getDepthTimeStamp() { return depthFrameTimestamp; }
	inline INT64 getColorTimeStamp() { return colorFrameTimestamp; }
	inline INT64 getInfraTimeStamp() { return infraFrameTimestamp; }

	inline USHORT getDepthMinReliableDistance() { return depthMinReliableDistance; }
	inline USHORT getDepthMaxReliableDistance() { return depthMaxReliableDistance; }

	// ��ɫͼ��1920 x 1080 @ 30 / 15 FPS�����ݻ������ȣ�
	const int colorImgHeight = 1080;
	const int colorImgWidth = 1920;

	// ���ͼ��512 x 424 @ 30 FPS��16bit ��Χ�� 0.5 ~4.5 M
	const int depthImgHeight = 424;
	const int depthImgWidth = 512;

	// ����ͼ�Σ�512 x 424 @ 30 FPS��16bit
	const int infraImgHeight = 424;
	const int infraImgWidth = 512;

private:
	HRESULT KinectInitialize(FrameTypes _type);

	HRESULT updateAll();
	HRESULT updateColorData();
	HRESULT updateDepthData();
	HRESULT updateInfraData();

	// Release function
	template< class T > inline void SafeRelease(T** ppT);
	// Safe release for interfaces
	template<class Interface> inline void SafeRelease(Interface *& pInterfaceToRelease);

	IKinectSensor *pSensor;
	IDepthFrameReader * pDepthFrameReader;
	IColorFrameReader * pColorFrameReader;
	IInfraredFrameReader * pInfraFrameReader;
	IMultiSourceFrameReader * pMultiSourceFrameReader;

	// frame event�� remember to clear event
	WAITABLE_HANDLE depthFrameEvent;
	WAITABLE_HANDLE colorFrameEvent;
	WAITABLE_HANDLE infraFrameEvent;
	WAITABLE_HANDLE allFramesEvent;

	// image frame time stamp, unit: 10^(-7) s// ʱ���
	INT64 depthFrameTimestamp;
	INT64 infraFrameTimestamp;
	INT64 colorFrameTimestamp;

	UINT depthBufferSize;
	UINT infraBufferSize;
	UINT colorBufferSize;

	USHORT depthMinReliableDistance;
	USHORT depthMaxReliableDistance;

	cv::Mat colorImg;
	cv::Mat depthImg;
	cv::Mat infraImg;
};


#endif // USING_KINECT
#endif // !_KREADER_H

