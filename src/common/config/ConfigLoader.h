/* XMRig
 * Copyright 2010      Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2012-2014 pooler      <pooler@litecoinpool.org>
 * Copyright 2014      Lucas Jones <https://github.com/lucasjones>
 * Copyright 2014-2016 Wolf9466    <https://github.com/OhGodAPet>
 * Copyright 2016      Jay D Dee   <jayddee246@gmail.com>
 * Copyright 2017-2018 XMR-Stak    <https://github.com/fireice-uk>, <https://github.com/psychocrypt>
 * Copyright 2016-2018 XMRig       <https://github.com/xmrig>, <support@xmrig.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __CONFIGLOADER_H__
#define __CONFIGLOADER_H__


#include <stdint.h>
#include <string>


#include "rapidjson/fwd.h"


struct option;


namespace xmrig {


class ConfigWatcher;
class IConfigCreator;
class IWatcherListener;
class IConfig;


class ConfigLoader
{
public:
    static bool loadFromFile(xmrig::IConfig *config, const char *fileName);
    static bool loadFromJSON(IConfig *config, const char *json);
    static bool loadFromJSON(IConfig *config, const rapidjson::Document &doc);
    static bool reload(IConfig *oldConfig, const char *json);
    static bool reload(xmrig::IConfig *oldConfig, const std::string &jsonConfig);
    static IConfig *load(const std::string &jsonConfig, IConfigCreator *creator, IWatcherListener *listener);
    static void release();

private:
    static bool getJSON(const std::string &jsonConfig, rapidjson::Document &doc);
    static bool parseArg(IConfig *config, int key, const char *arg);
    static void parseJSON(IConfig *config, const struct option *option, const rapidjson::Value &object);
    static void showUsage();
    static void showVersion();

    static ConfigWatcher *m_watcher;
    static IConfigCreator *m_creator;
    static IWatcherListener *m_listener;
};


} /* namespace xmrig */

#endif /* __CONFIGLOADER_H__ */
