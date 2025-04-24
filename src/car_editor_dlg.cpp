#include <car_editor_dlg.h>
#include <CarDef.h>
#include <sstream>
#include <string>

using namespace MODEL;

void floatSlider(const char *label, float *value, const min_max_values &limits)
{
    ImGui::SliderFloat(label, value, limits.m_min, limits.m_max);
}

std::string makeLabel(const char *label, const int suffix)
{
    std::stringstream ss;
    ss << label << " " << suffix;
    return ss.str();
}

void CCarEditorDlg::RenderContent()
{
    auto before = _carDef;
    ImGui::BeginGroup();
    {
        ImGui::Text("Peso 1");
        floatSlider("X##peso1", &_carDef._peso1.circle.x, car_value_limits::roda_x_min_max);
        floatSlider("Y##peso1", &_carDef._peso1.circle.y, car_value_limits::roda_y_min_max);
        floatSlider("Raio##peso1", &_carDef._peso1.circle.raio, car_value_limits::roda_r_min_max);
        floatSlider("Densidade##peso1", &_carDef._peso1.body.densidade, car_value_limits::roda_densidade_min_max);
        floatSlider("Fricção##peso1", &_carDef._peso1.body.friccao, car_value_limits::roda_friccao_min_max);
        floatSlider("Elasticidade##peso1", &_carDef._peso1.body.elasticidade, car_value_limits::roda_elasticidade_min_max);
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
        ImGui::Text("Peso 2");
        floatSlider("X##peso2", &_carDef._peso2.circle.x, car_value_limits::roda_x_min_max);
        floatSlider("Y##peso2", &_carDef._peso2.circle.y, car_value_limits::roda_y_min_max);
        floatSlider("Raio##peso2", &_carDef._peso2.circle.raio, car_value_limits::roda_r_min_max);
        floatSlider("Densidade##peso2", &_carDef._peso2.body.densidade, car_value_limits::roda_densidade_min_max);
        floatSlider("Fricção##peso2", &_carDef._peso2.body.friccao, car_value_limits::roda_friccao_min_max);
        floatSlider("Elasticidade##peso2", &_carDef._peso2.body.elasticidade, car_value_limits::roda_elasticidade_min_max);
    }
    ImGui::EndGroup();


    ImGui::BeginGroup();
    {
        ImGui::Text("Roda 1");
        floatSlider("X##roda1", &_carDef._roda1.circle.x, car_value_limits::roda_x_min_max);
        floatSlider("Y##roda1", &_carDef._roda1.circle.y, car_value_limits::roda_y_min_max);
        floatSlider("Raio##roda1", &_carDef._roda1.circle.raio, car_value_limits::roda_r_min_max);
        floatSlider("Densidade##roda1", &_carDef._roda1.body.densidade, car_value_limits::roda_densidade_min_max);
        floatSlider("Fricção##roda1", &_carDef._roda1.body.friccao, car_value_limits::roda_friccao_min_max);
        floatSlider("Elasticidade##roda1", &_carDef._roda1.body.elasticidade, car_value_limits::roda_elasticidade_min_max);
        floatSlider("Torque 1##roda1", &_carDef._torque[0], car_value_limits::torque_min_max);
        floatSlider("Torque 2##roda1", &_carDef._torque[2], car_value_limits::torque_min_max);
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
        ImGui::Text("Roda 2");
        floatSlider("X##roda2", &_carDef._roda2.circle.x, car_value_limits::roda_x_min_max);
        floatSlider("Y##roda2", &_carDef._roda2.circle.y, car_value_limits::roda_y_min_max);
        floatSlider("Raio##roda2", &_carDef._roda2.circle.raio, car_value_limits::roda_r_min_max);
        floatSlider("Densidade##roda2", &_carDef._roda2.body.densidade, car_value_limits::roda_densidade_min_max);
        floatSlider("Fricção##roda2", &_carDef._roda2.body.friccao, car_value_limits::roda_friccao_min_max);
        floatSlider("Elasticidade##roda2", &_carDef._roda2.body.elasticidade, car_value_limits::roda_elasticidade_min_max);
        floatSlider("Torque 1##roda2", &_carDef._torque[1], car_value_limits::torque_min_max);
        floatSlider("Torque 2##roda2", &_carDef._torque[3], car_value_limits::torque_min_max);
    }
    ImGui::EndGroup();


    ImGui::SameLine();
    ImGui::BeginGroup();
    {
    }
    ImGui::EndGroup();

    ImGui::BeginGroup();
    {
        ImGui::Text("Ligações");
        ImGui::BeginGroup();
        {
            ImGui::Text("Frequência");
            for( int i = 0; i < 6; i++)
              floatSlider(makeLabel("Frequência", i+1).c_str(), &_carDef._freq[i], car_value_limits::freq_min_max);

        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
            ImGui::Text("Amortecimento");
            for( int i = 0; i < 6; i++)
                floatSlider(makeLabel("Amortecimento", i+1).c_str(), &_carDef._damp[i], car_value_limits::damp_min_max);
        }
        ImGui::EndGroup();
    }
    ImGui::EndGroup();

    if (ImGui::Button("Run"))
    {
        _callback(_carDef);
    }
    ImGui::SameLine();
    if (ImGui::Button("Close"))
    {
        Hide();
    }
    ImGui::SameLine();
    ImGui::Checkbox("Auto-run", &_autoRun);

    if (_autoRun && before != _carDef)
    {
        _callback(_carDef);
    }
}
