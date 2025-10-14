#pragma once
#include <QObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>
#include <QImage>

extern "C"
{
#include <libavformat/avformat.h>
}

double getVideoFPS(const std::string &filePath);
int getVideoFrames(const std::string &filePath);
QString generateThumbnail(const QString &videoPath, int width = 160, int height = 90);

class MediaPoolManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList clips READ clips NOTIFY clipsChanged)

public:
    explicit MediaPoolManager(QObject *parent = nullptr)
        : QObject(parent)
    {
        // Automatically load when constructed
        load();
    }

    QVariantList clips() const { return m_clips; }

    Q_INVOKABLE void load()
    {
        QString path = getJsonPath();
        QFile file(path);

        if (!file.exists())
        {
            qWarning() << "⚠ Media pool not found, creating new:" << path;
            save(); // create empty file
            return;
        }

        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "❌ Could not open for reading:" << file.errorString();
            return;
        }

        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isArray())
        {
            qWarning() << "⚠ Invalid JSON, resetting file.";
            m_clips.clear();
            save();
            return;
        }

        m_clips.clear();
        for (const auto &clip : doc.array())
        {
            m_clips.append(clip.toObject().toVariantMap());
        }

        emit clipsChanged();
        qDebug() << "✅ Loaded" << m_clips.size() << "clips from" << path;
    }

    Q_INVOKABLE void save()
    {
        QString path = getJsonPath();
        QDir dir(QFileInfo(path).absolutePath());
        if (!dir.exists())
            dir.mkpath(".");

        QJsonArray array;
        for (const auto &clipVar : m_clips)
            array.append(QJsonObject::fromVariantMap(clipVar.toMap()));

        QJsonDocument doc(array);
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            qWarning() << "❌ Could not open for writing:" << file.errorString();
            return;
        }

        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();

        qDebug() << "✅ Saved media pool to" << path;
    }

    Q_INVOKABLE void addClip(const QString &name, const QString &url)
    {
        // Sanity check: avoid importing duplicates (compare normalized local file paths)
        QString newLocal = QUrl::fromUserInput(url).toLocalFile();
        if (!newLocal.isEmpty())
        {
            for (const auto &clipVar : m_clips)
            {
                QString existingUrl = clipVar.toMap().value("url").toString();
                QString existingLocal = QUrl::fromUserInput(existingUrl).toLocalFile();
                if (!existingLocal.isEmpty() && existingLocal == newLocal)
                {
                    qWarning() << "⚠ Clip already imported:" << newLocal;
                    return;
                }
            }
        }
        else
        {
            // fallback: compare raw url strings
            for (const auto &clipVar : m_clips)
            {
                if (clipVar.toMap().value("url").toString() == url)
                {
                    qWarning() << "⚠ Clip already imported (by URL):" << url;
                    return;
                }
            }
        }

        int frames = getVideoFrames(url.toStdString());
        QString thumbPath = generateThumbnail(url);
        QVariantMap clip;
        clip["name"] = name;
        clip["url"] = url;
        clip["fps"] = getVideoFPS(url.toStdString());
        clip["frames"] = frames;
        clip["thumbnail"] = QUrl::fromLocalFile(thumbPath).toString();
        m_clips.append(clip);
        emit clipsChanged();
    }

signals:
    void clipsChanged();

private:
    QVariantList m_clips;

    QString getJsonPath() const
    {
        QString exeDir = QCoreApplication::applicationDirPath();
        return exeDir + QDir::separator() + "media_pool.json";
    }
};
