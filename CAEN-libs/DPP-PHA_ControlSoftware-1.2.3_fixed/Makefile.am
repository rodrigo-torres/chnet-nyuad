AUTOMAKE_OPTIONS = foreign
SUBDIRS = src GUI

dist_noinst_HEADERS = inc/*.h


install-data-hook:
	chmod 777 /etc/DPP-PHA_ControlSoftware
	chmod 666 /etc/DPP-PHA_ControlSoftware/*.txt
	mv /etc/DPP-PHA_ControlSoftware/DefaultDppRunnerConfig.txt /etc/DPP-PHA_ControlSoftware/DppRunnerConfigDefault.txt
	mv /etc/DPP-PHA_ControlSoftware/Readme_PHA.txt /etc/DPP-PHA_ControlSoftware/Readme.txt
	mv /etc/DPP-PHA_ControlSoftware/ReleaseNotes_PHA.txt /etc/DPP-PHA_ControlSoftware/ReleaseNotes.txt
