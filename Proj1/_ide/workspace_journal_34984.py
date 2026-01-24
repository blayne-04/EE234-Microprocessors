# 2026-01-11T22:20:31.733000700
import vitis

client = vitis.create_client()
client.set_workspace(path="Proj1")

advanced_options = client.create_advanced_options_dict(dt_overlay="0")

platform = client.create_platform_component(name = "blackboard-platform",hw_design = "$COMPONENT_LOCATION/../../../../blackboard.xsa",os = "standalone",cpu = "ps7_cortexa9_0",domain_name = "standalone_ps7_cortexa9_0",generate_dtb = False,advanced_options = advanced_options,compiler = "gcc")

comp = client.create_app_component(name="proj1-app",platform = "$COMPONENT_LOCATION/../blackboard-platform/export/blackboard-platform/blackboard-platform.xpfm",domain = "standalone_ps7_cortexa9_0")

platform = client.get_component(name="blackboard-platform")
status = platform.build()

comp = client.get_component(name="proj1-app")
comp.build()

status = comp.clean()

vitis.dispose()

