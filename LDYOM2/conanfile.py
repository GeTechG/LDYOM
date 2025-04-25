import os

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.files import copy


class ldyomRecipe(ConanFile):
    name = "ldyom"
    version = "0.8"

    # Optional metadata
    license = "<Put the package license here>"
    author = "GeTechG"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of ldyom package here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*"

    def requirements(self):
        self.requires("plugin/0.0")
        self.requires("minhook/1.3.4")
        self.requires("imgui/1.91.8")
        self.requires("i18ncpp/1.0")
        self.requires("spdlog/1.15.1")
        self.requires("stduuid/1.2.3")
        self.requires("watcher/0.12.0")
        self.requires("luajit/2.1.0-beta3")

    def config_options(self):
        pass

    def configure(self):
        pass

    def layout(self):
        cmake_layout(self)
    
    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

        copy(self, "*win32*", os.path.join(self.dependencies["imgui"].package_folder,
            "res", "bindings"), os.path.join(self.source_folder, "src", "imgui"))
        copy(self, "*dx9*", os.path.join(self.dependencies["imgui"].package_folder,
            "res", "bindings"), os.path.join(self.source_folder, "src", "imgui"))
        copy(self, "*dx11*", os.path.join(self.dependencies["imgui"].package_folder,
            "res", "bindings"), os.path.join(self.source_folder, "src", "imgui"))
        copy(self, "*opengl3*", os.path.join(self.dependencies["imgui"].package_folder,
            "res", "bindings"), os.path.join(self.source_folder, "src", "imgui"))
        
        copy(self, "imgui_stdlib.*", os.path.join(self.dependencies["imgui"].package_folder,
            "res", "misc", "cpp"), os.path.join(self.source_folder, "src", "imgui"))
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
    
    def package(self):
        cmake = CMake(self)
        cmake.install()
