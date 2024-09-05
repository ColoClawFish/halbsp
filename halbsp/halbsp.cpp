// halbsp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//"C:\Program Files (x86)\Steam\steamapps\sourcemods\maplabstemplate" "C:\Program Files (x86)\Steam\steamapps\common\Source SDK Base 2013 Singleplayer" "C:\Users\Peter\Documents\assetexporttesting" "C:\Program Files (x86)\Steam\steamapps\sourcemods\maplabstemplate\maps/tt19_hal9000_1.bsp"
//"C:\Program Files (x86)\Steam\steamapps\sourcemods\eyecandy" "C:\Program Files (x86)\Steam\steamapps\common\Source SDK Base 2013 Singleplayer" "C:\Users\Peter\Documents\assetexporttesting" "C:\Program Files (x86)\Steam\steamapps\sourcemods\eyecandy\maps/tt08_monastery.bsp"

#include "halbsp.h"

char* memblock;
bsp* mapBsp;

vector<string> searchPaths;
vector<string> copiedModels;
vector<string> copiedMats;

bool replaceAll(string& str, const string& from, const string& to) {
    bool replaced = false;
    if (from.empty())
        return false;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        replaced = true;
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
    return replaced;
}

string findFilePath(string fileName) {
    for (int i = 0; i < searchPaths.size(); i++) {
        path p(searchPaths[i] + "/" + fileName);
        if (exists(p) && is_regular_file(p)) {
            return(p.generic_string());
        }
    }
    return "";
}

void copyFile(path filePath, path destinationFolder) {
    if (!exists(filePath)) return;
    /*
    if (exists(destinationFolder.append(filePath.filename()))) {
        cout << "Tried to copy file at " << filePath.string() << " to " << destinationFolder.string() << ", but it already exists." << endl;
    }
    */
    create_directories(destinationFolder);
    copy(filePath, destinationFolder, copy_options::overwrite_existing);
    cout << "Copying file at " << filePath.string() << " to " << destinationFolder.string() << endl;
}

KeyValueRoot& fileToKeyValues(string filename, KeyValueRoot& kv) {
    ifstream file(filename);
    //KeyValueRoot kv;

    if (!file.is_open()) {
        return kv;
    }

    string fileString;
    string line;
    while (getline(file, line)) {
        fileString += line + "\n";
    }

    for (string::size_type i = 0; i < fileString.length(); ++i) {
        fileString[i] = tolower(fileString[i]);
    }

    //cout << fileString;

    file.close();

    kv.Parse(fileString.c_str());
    cout << "File " << filename << " parsed to KVs.\n";

    return (kv);
}

void copyVmt(path vmtPath, path destinationFolder, path vmtBasePath) {
    if (!exists(vmtPath)) return;

    if (find(copiedMats.begin(), copiedMats.end(), vmtPath.string()) == copiedMats.end()) {
        copiedMats.push_back(vmtPath.string());
    }
    else {
        cout << "Material " << vmtPath.string() << " already copied." << endl;
        return;
    }

    path ndestinationFolder = destinationFolder;
    ndestinationFolder.append(vmtBasePath);

    copyFile(vmtPath, ndestinationFolder);

    KeyValueRoot kv;
    fileToKeyValues(vmtPath.generic_string().c_str(), kv);

    //cout << "Opened VMT: " << kv.ToString() << endl;

    //cout << "Opened VMT: " << kv.At(0).ToString() << endl;

    for (int i = 0; i < kv.At(0).ChildCount(); i++) {
        if (kv.At(0).At(i).IsValid()) {
            if (
                strcmp(kv.At(0).At(i).Key().string, "$basetexture") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$basetexture2") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$blendmodulatetexture") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$bumpmap") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$bumpmap2") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$bumpmask") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$lightwarptexture") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$ambientoccltexture") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$envmapmask") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$detail") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$detail1") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$detail2") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$selfillummask") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$selfillumtexture") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$emissiveblendtexture") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$emissiveblendflowtexture") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$normalmap") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$reflecttexture") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$refracttexture") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$flowmap") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$phongexponenttexture") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$phongwarptexture") == 0 ||
                strcmp(kv.At(0).At(i).Key().string, "$flow_noise_texture") == 0 
                ) {

                cout << "Texture: " << kv.At(0).At(i).Value().string << endl;

                //string filePath = "materials/" + (kv.At(0).At(i).Value().string) + ".vtf";
                string filePath = kv.At(0).At(i).Value().string;
                filePath = "materials/" + filePath;
                string nfilePath = findFilePath(filePath + ".vtf");
                //If unfound, it might already have the extension
                if (nfilePath == "")
                    nfilePath = findFilePath(filePath);

                if (nfilePath != "") {
                    cout << "Texture filepath " << nfilePath << endl;
                    path fPath(nfilePath);
                    path dPath = destinationFolder;
                    dPath.append(filePath);
                    dPath.remove_filename();

                    copyFile(fPath, dPath);
                }
                else {
                    cout << "Texture not found in paths" << endl;
                }
            }
        }
    }
}

void copyMdl(path mdlPath, path destinationFolder, path mdlBasePath) {
    //cout << mdlPath << endl << destinationFolder << endl << mdlBasePath << endl;
    if (!exists(mdlPath)) return;

    if (find(copiedModels.begin(), copiedModels.end(), mdlPath.string()) == copiedModels.end()) {
        copiedModels.push_back(mdlPath.string());
    }
    else {
        cout << "Model " << mdlPath.string() << " already copied." << endl;
        return;
    }

    destinationFolder = destinationFolder.append(mdlBasePath);
    //cout << destinationFolder << endl;

    copyFile(mdlPath, destinationFolder);

    mdlPath = mdlPath.replace_extension(".phy");
    copyFile(mdlPath, destinationFolder);

    mdlPath = mdlPath.replace_extension(".ani");
    copyFile(mdlPath, destinationFolder);

    mdlPath = mdlPath.replace_extension(".vtx");
    copyFile(mdlPath, destinationFolder);

    mdlPath = mdlPath.replace_extension(".vvd");
    copyFile(mdlPath, destinationFolder);

    mdlPath = mdlPath.replace_extension(".dx90.vtx");
    copyFile(mdlPath, destinationFolder);
}

mdl* openMdlFile(const char* fileName) {
    streampos size;
    mdl* _mdl;

    ifstream mdlFile (fileName, ios::in | ios::binary | ios::ate);

    char* mdlMemBlock = {};

    if (mdlFile.is_open()) {
        size = mdlFile.tellg();
        mdlMemBlock = new char[size];
        mdlFile.seekg(0, ios::beg);
        mdlFile.read(mdlMemBlock, size);
        mdlFile.close();

        _mdl = new mdl(mdlMemBlock);
    }
    else {
        _mdl = NULL;
        cout << "Error opening mdl " << fileName << "\n";
    }
    
    return _mdl;
}

void FindAndCopyModel(string oldModelName, string destinationPath) {
    string modelName = findFilePath(oldModelName);
    mdl* model = openMdlFile(modelName.c_str());
    if (model != NULL) {
        cout <<
            "Name: " << model->mdlStruct.header.name << "\n" <<
            "Texture Count: " << model->mdlStruct.header.texture_count << "\n";


        for (int i = 0; i < model->mdlStruct.header.texture_count; i++) {
            cout << "Texture " << i << ": " << model->mdlStruct.textureNameData[i] << "\n";

            string oldTextureFullDir = "materials/" + model->mdlStruct.textureDir + model->mdlStruct.textureNameData[i] + ".vmt";

            string textureFullDir = findFilePath(oldTextureFullDir);

            if (textureFullDir == "") {
                oldTextureFullDir = "materials/" + model->mdlStruct.textureNameData[i] + ".vmt";
                textureFullDir = findFilePath(oldTextureFullDir);
            }

            cout << "Full Path: " << textureFullDir << endl << endl;

            if (exists(textureFullDir)) {
                path vmtPath(textureFullDir);
                path destPath(destinationPath);
                path vmtBasePath(oldTextureFullDir);
                vmtBasePath.remove_filename();
                copyVmt(vmtPath, destPath, vmtBasePath);
            }
        }

        cout << "Texture Directory: " << model->mdlStruct.textureDir << "\n";

        cout << "Copying model " << modelName << endl;

        path p(modelName);
        path d(destinationPath);
        path mdlBasePath(oldModelName);
        mdlBasePath.remove_filename();
        copyMdl(p, d, mdlBasePath);
    }
    delete model;
}

void FindAndCopySound(string oldSoundName, string destinationPath) {
    while (
        oldSoundName.front() == '*' ||
        oldSoundName.front() == '#' ||
        oldSoundName.front() == '@' ||
        oldSoundName.front() == '>' ||
        oldSoundName.front() == '<' ||
        oldSoundName.front() == '^' ||
        oldSoundName.front() == ')' ||
        oldSoundName.front() == '}' ||
        oldSoundName.front() == '$' ||
        oldSoundName.front() == '!' ||
        oldSoundName.front() == '?' ||
        oldSoundName.front() == '&' ||
        oldSoundName.front() == '~' ||
        oldSoundName.front() == '`' ||
        oldSoundName.front() == '+' ||
        oldSoundName.front() == '%' ||
        oldSoundName.front() == '('
        ) {
        oldSoundName.erase(0, 1);
    }
    string soundName = findFilePath("sound/" + oldSoundName);
    if (soundName != "") {
        path soundPath(soundName);
        path oldSoundPath("sound/" + oldSoundName);
        path destPath(destinationPath);
        destPath.append(oldSoundPath.remove_filename());
        copyFile(soundPath, destPath);

        cout << "Copying sound " << soundName << " to " << destinationPath << endl;
    }
    else {
        cout << oldSoundName << " not found." << endl;
    }
}

void SearchSoundscriptForSound(string bspFilename, string destinationPath, string message) {
    path soundscriptPath(bspFilename);
    soundscriptPath.replace_filename(path(soundscriptPath.filename().replace_extension("")).string() + "_level_sounds.txt");
    cout << "Soundscript to search: " << soundscriptPath << endl;
    if (exists(soundscriptPath)) {
        KeyValueRoot soundscriptKV;
        fileToKeyValues(soundscriptPath.string(), soundscriptKV);
        copyFile(soundscriptPath, path(destinationPath + "/maps"));
        if (soundscriptKV.IsValid()) {
            cout << "Soundscript file valid." << endl;
            KeyValue soundentry = soundscriptKV.Get(message.c_str());
            if (soundentry.IsValid()) {
                KeyValue wavesoundentry = soundentry.Get("wave");
                if (wavesoundentry.IsValid()) {
                    FindAndCopySound(wavesoundentry.Value().string, destinationPath);
                }
                else {
                    wavesoundentry = soundentry.Get("rndwave");
                    if (wavesoundentry.IsValid()) {
                        for (int j = 0; j < wavesoundentry.ChildCount(); j++) {
                            FindAndCopySound(wavesoundentry.At(j).Value().string, destinationPath);
                        }
                    }
                }
            }
            else {
                cout << "Soundentry " << message << " not found!" << endl;
            }
        }
        else {
            cout << "Soundscript file invalid!" << endl;
        }
    }
    else {
        cout << "No map-specific soundscripts found." << endl;
    }
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        cout << "Please give a gameinfo path!\n";
        return -1;
    }

    if (argc < 3) {
        cout << "Please give an exe path!\n";
        return -1;
    }

    if (argc < 4) {
        cout << "Please give an destination path!\n";
        return -1;
    }

    if (argc < 5) {
        cout << "Please give a bsp filename!\n";
        return -1;
    }

    string executablePath = argv[2];
    executablePath += "/";
    
    string gameinfoPath = argv[1];
    gameinfoPath += "/";
    string gameinfoLocation = gameinfoPath + "gameinfo.txt";

    string destinationPath = argv[3];

    string bspFilename = argv[4];

    /*
    ifstream gameinfoFile(gameinfoLocation);

    if (!gameinfoFile.is_open()) {
        cout << "Gameinfo.txt not found at specified path!\n";
        return -1;
    }

    string gameinfoString;
    string line;
    while (getline(gameinfoFile, line)) {
        gameinfoString += line + "\n";
    }

    //cout << gameinfoString;

    gameinfoFile.close();

    KeyValueRoot& gameinfoKv(gameinfoString.c_str());
    */

    if (!exists(gameinfoLocation)) {
        cout << "Gameinfo.txt not found at specified path!\n";
        return -1;
    }

    KeyValueRoot gameinfoKv;
    fileToKeyValues(gameinfoLocation, gameinfoKv);

    //char printBuffer[1024];
    //gameinfoKv.ToString(printBuffer, 1024);
    //cout << printBuffer << endl;

    // << gameinfoKv["GameInfo"].Value().string << endl;

    int i = 0;
    while (gameinfoKv["GameInfo"]["FileSystem"]["SearchPaths"].At(i).IsValid()) {
        if (
            strcmp("platform", gameinfoKv["GameInfo"]["FileSystem"]["SearchPaths"].At(i).Key().string) != 0 &&
            strcmp("gamebin", gameinfoKv["GameInfo"]["FileSystem"]["SearchPaths"].At(i).Key().string) != 0
            ) {
            //cout << gameinfoKv["GameInfo"]["FileSystem"]["SearchPaths"].At(i).Key().string << "\t\t\t";
            //cout << gameinfoKv["GameInfo"]["FileSystem"]["SearchPaths"].At(i).Value().string << "\n";
            string value = gameinfoKv["GameInfo"]["FileSystem"]["SearchPaths"].At(i).Value().string;
            bool rawPath;
            rawPath = !replaceAll(value, "|gameinfo_path|", gameinfoPath) && !replaceAll(value, "|all_source_engine_paths|", executablePath);
            if (rawPath) {
                value = executablePath + value;
            }

            if (value[value.length() - 1] == '*') {
                value = value.substr(0, value.length() - 2);

                path p(value);

                if (exists(p)) {
                    if (is_directory(p)) {
                        for (directory_entry& x : directory_iterator(p)) {
                            if (is_directory(x.path())) {
                                cout << x.path().generic_string() << "\n";
                                searchPaths.emplace_back(x.path().generic_string());
                            }
                        }
                    }
                }
            }
            else {
                path p(value);
                if (exists(p)) {
                    if (is_directory(p)) {
                        cout << value << "\n";
                        searchPaths.emplace_back(value);
                    }
                }
            }
        }
        i++;
    }

    streampos size;

    ifstream bspFile (bspFilename, ios::in|ios::binary|ios::ate);

    if (bspFile.is_open()) {
        size = bspFile.tellg();
        memblock = new char[size];
        bspFile.seekg(0, ios::beg);
        bspFile.read(memblock, size);
        bspFile.close();

        mapBsp = new bsp(memblock);

        for (int i = 0; i < mapBsp->bspStruct.staticPropLumps.lumpEntries; i++) {
            cout << 
                "Model: " << mapBsp->bspStruct.staticPropDictLump.name[mapBsp->bspStruct.staticPropLumps.prop[i].PropType] << "\n" <<
                "Skin: " << mapBsp->bspStruct.staticPropLumps.prop[i].Skin << "\n";

            string oldModelName = mapBsp->bspStruct.staticPropDictLump.name[mapBsp->bspStruct.staticPropLumps.prop[i].PropType];
            FindAndCopyModel(oldModelName, destinationPath);

            cout << "\n";
        }

        string matname = "";
        for (int i = 0; i < mapBsp->bspStruct.header.lumps[LUMP_TEXDATA_STRING_DATA].filelen; i++) {
            if (mapBsp->bspStruct.texDataStringDataLump[i] == 0) {
                cout << "\n";

                if (matname.find("maps/" + path(bspFilename).filename().replace_extension("").string() + "/") == 0) {
                    int underlineCount = 0;
                    int index = matname.length() - 1;

                    while (index >= 0 && underlineCount < 3) {
                        if (matname.at(index) == '_') {
                            underlineCount++;
                        }
                        index--;
                    }

                    matname.erase(index + 1);
                    matname.erase(0, ("maps/" + path(bspFilename).filename().replace_extension("").string() + "/").length());
                }

                string oldTextureFullDir = "materials/" + matname + ".vmt";

                string textureFullDir = findFilePath(oldTextureFullDir);
                cout << "VMT Full Path: " << textureFullDir << endl;

                if (exists(textureFullDir)) {
                    path vmtPath(textureFullDir);
                    path destPath(destinationPath);
                    path vmtBasePath(oldTextureFullDir);
                    vmtBasePath.remove_filename();
                    copyVmt(vmtPath, destPath, vmtBasePath);
                }

                matname = "";
                continue;
            }
            cout << mapBsp->bspStruct.texDataStringDataLump[i];
            matname += tolower(mapBsp->bspStruct.texDataStringDataLump[i]);
            
            
        }

        //cout << "\n" << mapBsp->bspStruct.entityLump << "\n";
        
        string ent = "";
        int i = 1;
        while (i < mapBsp->bspStruct.header.lumps[LUMP_ENTITIES].filelen) {
            ent = "";
            while (mapBsp->bspStruct.entityLump[i] != '}') {
                ent += mapBsp->bspStruct.entityLump[i];
                i++;
            }

            KeyValueRoot entLumpKV;
            entLumpKV.Parse(ent.c_str());
            //cout << "\nEnt Lump:\n" << entLumpKV.ToString();
            string classname = "";
            string message = "";

            for (int j = 0; j < entLumpKV.ChildCount(); j++) {
                if (strcmp(entLumpKV.At(j).Key().string, "model") == 0) {
                    string modelPathString = entLumpKV.At(j).Value().string;
                    path modelPath = path(modelPathString);
                    if (modelPath.has_extension() && (strcmp(modelPath.extension().string().c_str(), ".mdl") == 0))
                    cout << "Model detected: " << modelPathString << endl;
                    FindAndCopyModel(modelPathString, destinationPath);
                }
                else if (strcmp(entLumpKV.At(j).Key().string, "classname") == 0) {
                    classname = entLumpKV.At(j).Value().string;
                }
                else if (strcmp(entLumpKV.At(j).Key().string, "message") == 0) {
                    message = entLumpKV.At(j).Value().string;
                }
            }

            if (classname == "ambient_generic" && message != "") {
                cout << "Sound detected " << message << endl;
                path soundPath = path(message);
                if (soundPath.has_extension() && (strcmp(soundPath.extension().string().c_str(), ".wav") == 0)) {
                    FindAndCopySound(message, destinationPath);
                }
                //Time to search the soundscripts!
                else {
                    SearchSoundscriptForSound(bspFilename, destinationPath, message);
                }
            }

            while (mapBsp->bspStruct.entityLump[i] != '{') {
                i++;
            }
            i++;
        }

        delete[] memblock;

        delete mapBsp;
    }
    else {
        cout << "Unable to open bsp" << endl;
    }
    return 420;
}