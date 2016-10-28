#pragma once

#include <QApplication>
#include <memory>
#include <QDebug>
#include <QFlag>
#include <QIcon>
#include <QDateTime>
#include <updater/GoUpdate.h>
class FolderInstanceProvider;

class GenericPageProvider;
class QFile;
class MinecraftVersionList;
class LWJGLVersionList;
class HttpMetaCache;
class SettingsObject;
class InstanceList;
class MojangAccountList;
class IconList;
class QNetworkAccessManager;
class ForgeVersionList;
class LiteLoaderVersionList;
class JavaInstallList;
class UpdateChecker;
class BaseProfilerFactory;
class BaseDetachedToolFactory;
class TranslationDownloader;
class ITheme;

#if defined(MMC)
#undef MMC
#endif
#define MMC (static_cast<MultiMC *>(QCoreApplication::instance()))

class MultiMC : public QApplication
{
	// friends for the purpose of limiting access to deprecated stuff
	friend class MultiMCPage;
	friend class MainWindow;
	Q_OBJECT
public:
	enum Status
	{
		Failed,
		Succeeded,
		Initialized
	};

public:
	MultiMC(int &argc, char **argv);
	virtual ~MultiMC();

	// InstanceList, IconList, OneSixFTBInstance, LegacyUpdate, LegacyInstance, MCEditTool, JVisualVM, MinecraftInstance, JProfiler, BaseInstance
	std::shared_ptr<SettingsObject> settings()
	{
		return m_settings;
	}

	std::shared_ptr<GenericPageProvider> globalSettingsPages()
	{
		return m_globalSettingsProvider;
	}

	qint64 timeSinceStart() const
	{
		return startTime.msecsTo(QDateTime::currentDateTime());
	}

	QIcon getThemedIcon(const QString& name);

	void setIconTheme(const QString& name);

	std::vector<ITheme *> getValidApplicationThemes();
	void setApplicationTheme(const QString& name);

	// DownloadUpdateTask
	std::shared_ptr<UpdateChecker> updateChecker()
	{
		return m_updateChecker;
	}

	std::shared_ptr<MinecraftVersionList> minecraftlist();
	std::shared_ptr<LWJGLVersionList> lwjgllist();
	std::shared_ptr<ForgeVersionList> forgelist();
	std::shared_ptr<LiteLoaderVersionList> liteloaderlist();
	std::shared_ptr<JavaInstallList> javalist();

	// APPLICATION ONLY
	std::shared_ptr<InstanceList> instances() const
	{
		return m_instances;
	}

	FolderInstanceProvider * folderProvider() const
	{
		return m_instanceFolder;
	}

	std::shared_ptr<IconList> icons() const
	{
		return m_icons;
	}

	// APPLICATION ONLY
	std::shared_ptr<MojangAccountList> accounts() const
	{
		return m_accounts;
	}

	// APPLICATION ONLY
	Status status() const
	{
		return m_status;
	}

	// APPLICATION ONLY
	const QMap<QString, std::shared_ptr<BaseProfilerFactory>> &profilers() const
	{
		return m_profilers;
	}

	// APPLICATION ONLY
	const QMap<QString, std::shared_ptr<BaseDetachedToolFactory>> &tools() const
	{
		return m_tools;
	}

	// APPLICATION ONLY
	void installUpdates(const QString updateFilesDir, GoUpdate::OperationList operations);

	/*!
	 * Opens a json file using either a system default editor, or, if note empty, the editor
	 * specified in the settings
	 */
	bool openJsonEditor(const QString &filename);

protected: /* to be removed! */
	// FIXME: remove. used by MainWindow to create application update tasks
	/// this is the root of the 'installation'. Used for automatic updates
	const QString &root()
	{
		return rootPath;
	}

private slots:
	/**
	 * Do all the things that should be done before we exit
	 */
	void onExit();

private:
	void initLogger();
	void initIcons();
	void initThemes();
	void initGlobalSettings();
	void initTranslations();
	void initSSL();

private:
	QDateTime startTime;

	std::shared_ptr<QTranslator> m_qt_translator;
	std::shared_ptr<QTranslator> m_mmc_translator;
	std::shared_ptr<SettingsObject> m_settings;
	std::shared_ptr<InstanceList> m_instances;
	FolderInstanceProvider * m_instanceFolder = nullptr;
	std::shared_ptr<IconList> m_icons;
	std::shared_ptr<UpdateChecker> m_updateChecker;
	std::shared_ptr<MojangAccountList> m_accounts;
	std::shared_ptr<LWJGLVersionList> m_lwjgllist;
	std::shared_ptr<ForgeVersionList> m_forgelist;
	std::shared_ptr<LiteLoaderVersionList> m_liteloaderlist;
	std::shared_ptr<MinecraftVersionList> m_minecraftlist;
	std::shared_ptr<JavaInstallList> m_javalist;
	std::shared_ptr<TranslationDownloader> m_translationChecker;
	std::shared_ptr<GenericPageProvider> m_globalSettingsProvider;
	std::map<QString, std::unique_ptr<ITheme>> m_themes;

	QMap<QString, std::shared_ptr<BaseProfilerFactory>> m_profilers;
	QMap<QString, std::shared_ptr<BaseDetachedToolFactory>> m_tools;

	QString rootPath;

	Status m_status = MultiMC::Failed;

	bool consoleAttached = false;
public:
	QString instanceIdToLaunch;
	std::unique_ptr<QFile> logFile;
};
