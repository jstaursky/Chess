#include "Utility/FileDialog.h"

#include <vector>
#include <iostream>

#include <unistd.h>
#include <linux/limits.h>

#include "ImGuiFileDialog/ImGuiFileDialog.h"

namespace FileDialog {


std::string Open()
{
    ImGuiFileDialog::Instance()->OpenDialog ("ChooseFileDlgKey", "Choose File", ".*", ".");
    return "";
}

std::string Open (char *buffer, size_t size)
{
    // ImGui::OpenPopup ("Choose File");
    // ImGuiFileDialog::Instance()->OpenDialog ("ChooseFileDialogKey", "Choose File", "*", ".");

    return "";

}

std::string Save()
{
    return "";

}


}
