QT          += core gui widgets

TEMPLATE    = app
CONFIG      += c++17

TARGET      = Oblig3Visualisering

PRECOMPILED_HEADER = innpch.h

INCLUDEPATH +=  ./GSL \
                ./Components \
                ./Systems   \
                ./Widgets


mac {
    LIBS += -framework OpenAL
}
# windows
win32 {
    INCLUDEPATH += $(OPENAL_HOME)\\include\\AL

    # 32 bits windows compiler
    contains(QT_ARCH, i386) {
        LIBS *= $(OPENAL_HOME)\\libs\\Win32\\libOpenAL32.dll.a

        CONFIG(debug, debug|release) {
            OpenAL32.commands = copy /Y \"$(OPENAL_HOME)\\bin\\Win32\\OpenAL32.dll\" debug\\OpenAL32.dll
            OpenAL32.target = debug/OpenAL32.dll

            QMAKE_EXTRA_TARGETS += OpenAL32
            PRE_TARGETDEPS += debug/OpenAL32.dll
        } else:CONFIG(release, debug|release) {
            OpenAL32.commands = copy /Y \"$(OPENAL_HOME)\\bin\\Win32\\OpenAL32.dll\" release\\OpenAL32.dll
            OpenAL32.target = release/OpenAL32.dll

            QMAKE_EXTRA_TARGETS += OpenAL32
            PRE_TARGETDEPS += release/OpenAL32.dll
        }
    # 64 bits windows compiler
    } else {
        LIBS *= $(OPENAL_HOME)\\libs\\Win64\\libOpenAL32.dll.a

        CONFIG(debug, debug|release) {
            OpenAL32.commands = copy \"$(OPENAL_HOME)\\bin\\Win64\\OpenAL32.dll\" debug\\OpenAL32.dll
            OpenAL32.target = debug/OpenAL32.dll

            QMAKE_EXTRA_TARGETS += OpenAL32
            PRE_TARGETDEPS += debug/OpenAL32.dll
        } else:CONFIG(release, debug|release) {
            OpenAL32.commands = copy /Y \"$(OPENAL_HOME)\\bin\\Win64\\OpenAL32.dll\" release\\OpenAL32.dll
            OpenAL32.target = release/OpenAL32.dll

            QMAKE_EXTRA_TARGETS += OpenAL32
            PRE_TARGETDEPS += release/OpenAL32.dll
        }
    }
}


HEADERS += \
    Components/collisioncomponent.h \
    Components/components.h \
    Components/inputcomponent.h \
    Components/materialcomponent.h \
    Components/meshcomponent.h \
    Components/rendercomponent.h \
    Components/soundcomponent.h \
    Components/transformcomponent.h \
    FSM/bsplinecurve.h \
    FSM/npc.h \
    GSL/matrix.h \
    GSL/matrix2x2.h \
    GSL/matrix3x3.h \
    GSL/matrix4x4.h \
    GSL/quaternion.h \
    GSL/vector2d.h \
    GSL/vector3d.h \
    GSL/vector4d.h \
    GSL/gsl_math.h \
    GSL/math_constants.h \
    Physics/gravity.h \
    Physics/raytracing.h \
    Scenes/scene.h \
    Systems/collisionsystem.h \
    Systems/componentsystem.h \
    Systems/inputsystem.h \
    Systems/rendersystem.h \
    Systems/system.h \
    Systems/systemmanager.h \
    Triangulation/regular.h \
    Widgets/entitywidget.h \
    Widgets/transformwidget.h \
    constants.h \
    engine.h \
    entity.h \
    mousepicker.h \
    renderwindow.h \
    resourcefactory.h \
    shader.h \
    mainwindow.h \
    soundmanager.h \
    soundsource.h \
    triangle.h \
    texture.h \
    billboard.h \
    vertex.h \
    wavefilehandler.h \
    xyz.h \
    visualobject.h \
    octahedronball.h \
    billboard.h \
    camera.h \
    octahedronball.h \
    gltypes.h \
    trianglesurface.h \
    input.h \
    material.h \
    light.h \
    objmesh.h \
#    innpch.h \
    colorshader.h \
    textureshader.h \
    skybox.h \
    phongshader.h \


SOURCES += main.cpp \
    Components/collisioncomponent.cpp \
    Components/components.cpp \
    Components/inputcomponent.cpp \
    Components/materialcomponent.cpp \
    Components/meshcomponent.cpp \
    Components/rendercomponent.cpp \
    Components/soundcomponent.cpp \
    Components/transformcomponent.cpp \
    FSM/bsplinecurve.cpp \
    FSM/npc.cpp \
    GSL/matrix2x2.cpp \
    GSL/matrix3x3.cpp \
    GSL/matrix4x4.cpp \
    GSL/quaternion.cpp \
    GSL/vector2d.cpp \
    GSL/vector3d.cpp \
    GSL/vector4d.cpp \
    GSL/gsl_math.cpp \
    Physics/gravity.cpp \
    Physics/raytracing.cpp \
    Scenes/scene.cpp \
    Systems/collisionsystem.cpp \
    Systems/componentsystem.cpp \
    Systems/inputsystem.cpp \
    Systems/rendersystem.cpp \
    Systems/system.cpp \
    Systems/systemmanager.cpp \
    Triangulation/regular.cpp \
    Widgets/entitywidget.cpp \
    Widgets/transformwidget.cpp \
    engine.cpp \
    entity.cpp \
    mousepicker.cpp \
    renderwindow.cpp \
    mainwindow.cpp \
    resourcefactory.cpp \
    shader.cpp \
    soundmanager.cpp \
    soundsource.cpp \
    triangle.cpp \
    texture.cpp \
    billboard.cpp \
    vertex.cpp \
    visualobject.cpp \
    camera.cpp \
    octahedronball.cpp \
    wavefilehandler.cpp \
    xyz.cpp \
    trianglesurface.cpp \
    input.cpp \
    material.cpp \
    light.cpp \
    objmesh.cpp \
    colorshader.cpp \
    textureshader.cpp \
    skybox.cpp \
    phongshader.cpp

FORMS += \
    EntityWidget.ui \
    mainwindow.ui \
    meshComponentWidget.ui \
    transformComponentWidget.ui

DISTFILES += \
    Shaders/phongshader.frag \
    Shaders/phongshader.vert \
    Shaders/plainshader.frag \
    Shaders/plainshader.vert \
    Shaders/textureshader.frag \
    GSL/README.md \
    README.md \
    Shaders/textureshader.vert
