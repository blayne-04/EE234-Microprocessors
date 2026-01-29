# 2026-01-28T18:34:18.852577200
import vitis

client = vitis.create_client()
client.set_workspace(path="Proj1")

platform = client.get_component(name="blackboard-platform")
status = platform.build()

comp = client.get_component(name="proj1-app")
comp.build()

vitis.dispose()

