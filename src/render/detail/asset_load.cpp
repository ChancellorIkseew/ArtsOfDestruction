#pragma once
#include "bgfx_shader.hpp"
//
#include <fstream>
#include <SDL3/SDL_surface.h>
#include "debug/logger.hpp"

static debug::Logger logger("shader_load");

static std::string readFile(const fs::path& path) {
    if (!fs::exists(path) || !fs::is_regular_file(path)) {
        logger.error() << "File does not exist: " << fs::absolute(path);
        return std::string();
    }
    //
    std::ifstream fin(path, std::ios::binary | std::ios::ate);
    if (!fin.is_open()) {
        logger.error() << "Failed to open file: " << fs::absolute(path);
        return std::string();
    }
    //
    std::streamsize size = fin.tellg();
    fin.seekg(0, std::ios::beg);
    std::string str;
    str.resize(size);
    fin.read(str.data(), size);
    return str;
}

bgfx::ShaderHandle loadShader(const fs::path& path) {
    const std::string str = readFile(path);
    const bgfx::Memory* mem = bgfx::copy(str.data(), static_cast<uint32_t>(str.size()));
    bgfx::ShaderHandle shader = bgfx::createShader(mem);
    if (!bgfx::isValid(shader))
        logger.error() << "Shader corrupted: " << path.filename();
    return shader;
}

bgfx::TextureHandle loadTextureWithSDL(const fs::path& path) {
    // 1. Загружаем через SDL3
    SDL_Surface* surface = SDL_LoadPNG(path.string().c_str());
    if (!surface)
        return BGFX_INVALID_HANDLE;

    // 2. Конвертируем в RGBA32, если формат другой (важно для предсказуемости)
    SDL_Surface* converted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(surface);

    if (!converted)
        return BGFX_INVALID_HANDLE;

    // 3. Копируем данные в память, которую bgfx отправит на GPU
    const bgfx::Memory* mem = bgfx::copy(converted->pixels, converted->pitch * converted->h);

    // 4. Создаем текстуру
    bgfx::TextureHandle handle = bgfx::createTexture2D(
        (uint16_t)converted->w,
        (uint16_t)converted->h,
        false, 1,
        bgfx::TextureFormat::RGBA8,
        BGFX_TEXTURE_NONE,
        mem
    );

    SDL_DestroySurface(converted);
    return handle;
}

#include "render/detail/bgfx_renderer.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

ModelData loadOBJ(const fs::path& path) {
    const std::string obj = readFile(path);
    const std::string mtl = "";
    tinyobj::ObjReader reader;
    reader.ParseFromString(obj, mtl);
    tinyobj::attrib_t attrib = reader.GetAttrib();
    ModelData data;
    
    for (const auto& shape : reader.GetShapes()) {
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {

            // Мы точно знаем, что здесь 3 вершины (треугольник), так как была триангуляция
            for (size_t v_idx = 0; v_idx < 3; v_idx++) {
                // ВАЖНО: для bgfx (Left-Hand) меняем порядок обхода: 0, 2, 1 вместо 0, 1, 2
                size_t remapped_v = (v_idx == 2) ? 2 : (v_idx == 1) ? 1 : 0;

                tinyobj::index_t idx = shape.mesh.indices[3 * f + remapped_v];

                Vertex vertex;

                // 1. Позиции (Инвертируем X для вашей системы координат)
                vertex.x = -attrib.vertices[3 * idx.vertex_index + 0];
                vertex.y = attrib.vertices[3 * idx.vertex_index + 1];
                vertex.z = attrib.vertices[3 * idx.vertex_index + 2];

                // 2. Цвет (по умолчанию белый)
                vertex.color = 0xffffffff;

                // 3. Текстурные координаты (UV)
                if (idx.texcoord_index >= 0) {
                    vertex.u = attrib.texcoords[2 * idx.texcoord_index + 0];
                    vertex.v = 1.0f - attrib.texcoords[2 * idx.texcoord_index + 1]; // Инверсия V для DirectX/bgfx
                }
                else {
                    vertex.u = vertex.v = 0.0f;
                }

                // 4. Нормали (Инвертируем X вслед за позицией)
                if (idx.normal_index >= 0) {
                    vertex.nx = attrib.normals[3 * idx.normal_index + 0];
                    vertex.ny = attrib.normals[3 * idx.normal_index + 1];
                    vertex.nz = -attrib.normals[3 * idx.normal_index + 2];
                }
                else {
                    // Если нормалей нет, ставим "вверх", чтобы не было черного мусора
                    vertex.nx = 0.0f; vertex.ny = 1.0f; vertex.nz = 0.0f;
                }

                data.vertices.push_back(vertex);
                data.indices.push_back(static_cast<uint32_t>(data.vertices.size() - 1));
            }
        }
    }


    return data;
}


/*
ModelData loadOBJ(const fs::path& path) {
    ModelData data;
    std::vector<FPoint3D> temp_positions;
    std::vector<FPoint2D> temp_uvs;
    std::vector<FPoint3D> temp_normals;

    std::ifstream file(path);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            FPoint3D v;
            ss >> v.x >> v.y >> v.z;
            temp_positions.push_back(v);
        }
        else if (prefix == "vt") {
            FPoint2D vt;
            ss >> vt.x >> vt.y;
            temp_uvs.push_back(vt);
        }
        else if (prefix == "vn") {
            FPoint3D vn;
            ss >> vn.x >> vn.y >> vn.z;
            temp_normals.push_back(vn);
        }
        else if (prefix == "f") {
            for (int i = 0; i < 3; ++i) {
                std::string vertexStr;
                ss >> vertexStr;

                // Формат f: v/vt/vn
                int vIdx, vtIdx, vnIdx;
                replace(vertexStr.begin(), vertexStr.end(), '/', ' ');
                std::stringstream vss(vertexStr);
                vss >> vIdx >> vtIdx >> vnIdx;

                Vertex v;
                // OBJ индексы начинаются с 1
                v.x = -temp_positions[vIdx - 1].x;
                v.y = temp_positions[vIdx - 1].y;
                v.z = temp_positions[vIdx - 1].z;
                v.u = temp_uvs[vtIdx - 1].x;
                v.v = temp_uvs[vtIdx - 1].y;
                v.color = 0xffffffff;
                v.nx = -temp_normals[vIdx - 1].x;
                v.ny = temp_normals[vIdx - 1].y;
                v.nz = temp_normals[vIdx - 1].z;

                data.vertices.push_back(v);
                data.indices.push_back(static_cast<uint16_t>(data.vertices.size() - 1));
            }
        }
    }
    return data;
}
*/
