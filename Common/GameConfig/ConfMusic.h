#ifndef __CONF_MUSIC_H__
#define __CONF_MUSIC_H__

#include "ConfManager.h"

class BgMusicItem
{
public:
    int BgMusicID;           // ��������ID
    int FadeInTime;          // ����ʱ��(����)
    int FadeOutTime;         // ����ʱ��(����)
    int IsRepeate;           // �Ƿ�ѭ��
    std::string FileName;    // �ļ���
};

class BgMusicSetting
{
public:
    int UIID;                // ������ʶID
    int BgMusicID;           // ��Ӧ�ı�������ID
    int EffectID;            // ��UI������Ч
    std::vector<std::string> MoodEffect; // ��UI���ŷ�Χ��Ч
};

class AudioItem
{
public:
    int         AudioID;                 // ��Чid
    std::string AudioPath;               // ��Ч·��
    bool        isRuleAffect;            // �Ƿ��ܵ�������Ч���Ź���Ӱ��
};

///////////////////////////////// ��ȡ /////////////////////////////////////////
class CConfBgMusic : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfBgMusicSetting : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
};

class CConfUIButtonEffect : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
    int getButtonEffectId(std::string btnName);

protected:
    std::map<std::string, int> m_ButtonEffect;
};

class CConfUISoundEffect : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
    bool getEffectPath(int effectID, std::string& path);

protected:
    std::map<int, std::string> m_EffectPath;
};

class CConfAudio : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
};

////////////////////////////////// ��ѯ ////////////////////////////////////////
inline const BgMusicItem* queryConfBgMusic(int bmgID)
{
    CConfBgMusic *conf = dynamic_cast<CConfBgMusic*>(
        CConfManager::getInstance()->getConf(CONF_BG_MUSIC));
    return static_cast<BgMusicItem*>(conf->getData(bmgID));
}

inline const BgMusicSetting* queryConfBgMusicSetting(int uiID)
{
    CConfBgMusicSetting *conf = dynamic_cast<CConfBgMusicSetting*>(
        CConfManager::getInstance()->getConf(CONF_BG_MUSIC_SETTING));
    return static_cast<BgMusicSetting*>(conf->getData(uiID));
}

inline const AudioItem* queryConfAudio(int musicId)
{
    CConfAudio *conf = dynamic_cast<CConfAudio*>(
        CConfManager::getInstance()->getConf(CONF_AUDIO));
    return static_cast<AudioItem*>(conf->getData(musicId));
}

#endif