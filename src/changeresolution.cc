
#include "../../ConfigReader/src/configreader.h"
#include "../../IncludeSpdlog/spdlog.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QDir>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

constexpr auto GENERAL{ "General" };
constexpr auto GENETIC{ "Genetic" };
constexpr auto DATASET{ "DataSet" };
constexpr auto LOGLEVEL{ "LogLevel" };
constexpr auto CHANGE{ "ChangeResolution" };

constexpr auto INPUT_FOLDER{ "InputFolder" };
constexpr auto OUTPUT_FOLDER{ "OutputFolder" };
constexpr auto CODEC{ "Codec" };
constexpr auto WIDTH{ "Width" };
constexpr auto HEIGHT{ "Height" };
constexpr auto FPS{ "FPS" };
constexpr auto RESIZE{ "Resize" };

using namespace std;

int main(int argc, char *argv[])
{
  QString configPath{ "" };
  QString configName{ "config.json" };

  ConfigReader configReader; 
  QJsonObject jObject = configReader.readConfig(configPath, configName);

  QJsonObject jGeneral{ jObject[GENERAL].toObject() };
  QJsonObject jChange{ jObject[CHANGE].toObject() };
  QString m_InputFolder = jChange["InputFolder"].toString();
  QString m_OutputFolder = jChange["OutputFolder"].toString();
  qint32 m_width = jChange[WIDTH].toInt();
  qint32 m_height = jChange[HEIGHT].toInt();
  double m_fps = jChange[FPS].toDouble();
  bool m_resize = jChange[RESIZE].toBool();

  QDir configsFolder = QDir(m_InputFolder);
  QStringList m_configList = configsFolder.entryList(QDir::Files);

  H_Logger->info("Client() searach mp4 in {}:" ,m_InputFolder.toStdString());
  H_Logger->info("Client() found {} config" ,m_configList.size());
  for(qint32 i = 0 ; i < m_configList.size() ; i++)
  {
     H_Logger->info("Change resolution in:{}" ,m_configList[i].toStdString());
    cv::VideoCapture cap((m_InputFolder+m_configList[i]).toStdString()); 

    std::string codecSTR =  jChange[CODEC].toString().toStdString(); //"avc1";
    qint32 m_code = cv::VideoWriter::fourcc(codecSTR[0], codecSTR[1], codecSTR[2], codecSTR[3]);
    cv::VideoWriter m_videoShoal;
    m_videoShoal = { (m_OutputFolder+m_configList[i]).toStdString(), cv::CAP_FFMPEG, m_code, m_fps, cv::Size(m_width, m_height), true };
        
    while(1)
    {
      cv::Mat frame;
      cv::Mat inputMatResize;
      cap >> frame;
      if (frame.empty())
        {break;}
      
      //resize:
      if(m_resize)
        {cv::resize(frame, inputMatResize, cv::Size(m_width, m_height));}
      else
      {
        cv::Rect region_of_interest = cv::Rect(0, 0, m_width, m_height);
        inputMatResize = frame(region_of_interest);
        
      }

      //write:
      m_videoShoal.write(inputMatResize);
    }
    m_videoShoal.release();
    cap.release();
  }

  // Closes all the frames
  cv::destroyAllWindows();

  return 0;
}
