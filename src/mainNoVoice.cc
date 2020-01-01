#include "../../IncludeSpdlog/spdlog.h"

#include <QFile>
#include <QDir>
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
  QString m_wpath = jGeneral["WritePath"].toString();
  double m_fps = (double)jGeneral["FPS"].toInt();
  QString m_type = jGeneral["Type"].toString();
  qint32 m_width = jGeneral["Width"].toInt();
  qint32 m_height = jGeneral["Height"].toInt();
  qint32 m_sec = jGeneral["Sec"].toInt();
  cv::VideoWriter m_videoShoal;

  H_Logger->set_level(static_cast<spdlog::level::level_enum>(messageLevel));
  H_Logger->set_pattern("[%Y-%m-%d] [%H:%M:%S.%e] [%t] [%^%l%$] %v");
  H_Logger->debug("start main logger");

    //assume the directory exists and contains some files and you want all jpg and JPG files
    QDir directory(m_path);
    QStringList images = directory.entryList(QStringList() << "*.mp4" << "*.MP4",QDir::Files);
    foreach(QString filename, images) 
    {
        H_Logger->debug("filename:{}",filename.toStdString().c_str());
        cv::VideoCapture cap((m_path+filename).toStdString()); 
        H_Logger->trace("inputfile:{}", (filename).toStdString().c_str());
        H_Logger->trace("m_type:{}", (m_type).toStdString().c_str());
        H_Logger->trace("m_path:{}", (m_path).toStdString().c_str());

        if(!cap.isOpened()){
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
        }
        double fps = cap.get(cv::CAP_PROP_FPS);
        std::cout << "Frames per second using cap.get(CV_CAP_PROP_FPS) : " << fps << std::endl;

        int iterVideo = 0;
        int deltaFrames = 30;
        int outputFrames = 30 * m_sec;


        while(1)
        {
            cv::Mat frame;
            cap >> frame;
            if (frame.empty())
                break;

            if(iterVideo == 0)
            {
                iterVideo++;
                if(m_width == 0)
                {
                    m_width = frame.cols;
                }
                if(m_height == 0)
                {
                    m_height = frame.rows;
                }
                m_videoShoal = { (m_wpath+filename).toStdString(), cv::CAP_FFMPEG, m_code, m_fps, cv::Size(m_width, m_height), true };
                H_Logger->trace("name:{}", (m_wpath+filename).toStdString().c_str());

            }
            else
            {
                m_videoShoal.write(frame);
            }
        }
        cap.release();

    }
    cv::destroyAllWindows();
    return 0;
}
