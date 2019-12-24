#include "../../IncludeSpdlog/spdlog.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

constexpr auto GENERAL{ "General" };
constexpr auto GENETIC{ "Genetic" };
constexpr auto DATASET{ "DataSet" };
constexpr auto LOGLEVEL{ "LogLevel" };
constexpr auto FILENAME{ "Filename" };

using namespace std;

int main(int argc, char *argv[])
{

  QString prefix = "CONFIG_";
  if (argc > 1)
  {
    std::string aa = argv[1];
    prefix = QString::fromStdString(aa);
  }

  QString configDir{ "config.json" };
  QFile jConfigFile{ configDir };
  if (!jConfigFile.open(QIODevice::ReadOnly))
  {
    qFatal("Failed to load config.json! :(");
  }

  QJsonDocument jConfigDoc{ QJsonDocument::fromJson((jConfigFile.readAll())) };
  if (!jConfigDoc.isObject())
  {
    qFatal("Invalid json config file!");
  }
  QJsonObject jObject{ jConfigDoc.object() };

  QJsonObject jGeneral{ jObject[GENERAL].toObject() };

  auto messageLevel{ jGeneral[LOGLEVEL].toInt() };
  QString filename{ jGeneral[FILENAME].toString() };
  std::string codecSTR =  jGeneral["Codec"].toString().toStdString(); //"avc1";
  qint32 m_code = cv::VideoWriter::fourcc(codecSTR[0], codecSTR[1], codecSTR[2], codecSTR[3]);
  QString m_path = jGeneral["Path"].toString();
  double m_fps = (double)jGeneral["FPS"].toInt();
  QString m_type = jGeneral["Type"].toString();
  qint32 m_width = jGeneral["Width"].toInt();
  qint32 m_height = jGeneral["Height"].toInt();
  qint32 m_sec = jGeneral["Sec"].toInt();
  cv::VideoWriter m_videoShoal;


  H_Logger->set_level(static_cast<spdlog::level::level_enum>(messageLevel));
  H_Logger->set_pattern("[%Y-%m-%d] [%H:%M:%S.%e] [%t] [%^%l%$] %v");
  H_Logger->debug("start main logger");

  cv::VideoCapture cap((m_path+filename+m_type).toStdString()); 
  H_Logger->trace("inputfile:{}", (filename+m_type).toStdString().c_str());
  H_Logger->trace("m_type:{}", (m_type).toStdString().c_str());
  H_Logger->trace("m_path:{}", (m_path).toStdString().c_str());
  // Check if camera opened successfully
  if(!cap.isOpened()){
    std::cout << "Error opening video stream or file" << std::endl;
    return -1;
  }
    
    double fps = cap.get(cv::CAP_PROP_FPS);
    // If you do not care about backward compatibility
    // You can use the following instead for OpenCV 3
    // double fps = cap.get(CAP_PROP_FPS);
    cout << "Frames per second using cap.get(CV_CAP_PROP_FPS) : " << fps << endl;

  int iter = 0;
  int iterVideo = 0;
  int deltaFrames = 30;
  int outputFrames = 30 * m_sec;
  qint32 m_iter = 0;


  while(1)
  {
    iter++;
    cv::Mat frame;
    cap >> frame;
    if (frame.empty())
      break;

    if(iter > deltaFrames)
    {
      if(iterVideo == 0)
      {
        m_iter++;
        iterVideo++;
        // Check if width or height is zero:
        if(m_width == 0)
        {
          m_width = frame.cols;
        }
        if(m_height == 0)
        {
          m_height = frame.rows;
        }


       
        m_videoShoal = { (m_path+filename+"_"+QString::number(m_iter)+m_type).toStdString(), cv::CAP_FFMPEG, m_code, m_fps, cv::Size(m_width, m_height), true };
        H_Logger->trace("name:{}", (m_path+filename+"_"+QString::number(m_iter)+m_type).toStdString().c_str());
    
      }
      else
      {
        iterVideo++;
        m_videoShoal.write(frame);
        if((iterVideo >= outputFrames))
        {
          H_Logger->trace("new video:");
          iterVideo=0;
          m_videoShoal.release();
        }
      }
    }

  }
  
  // When everything done, release the video capture object
  cap.release();
 
  // Closes all the frames
  cv::destroyAllWindows();

  return 0;
}
