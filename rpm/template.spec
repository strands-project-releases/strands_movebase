Name:           ros-indigo-strands-movebase
Version:        0.0.6
Release:        0%{?dist}
Summary:        ROS strands_movebase package

Group:          Development/Libraries
License:        MIT
URL:            http://github.com/strands-project/strands_movebase
Source0:        %{name}-%{version}.tar.gz

Requires:       pcl
Requires:       pcl-tools
Requires:       ros-indigo-pcl-ros
Requires:       ros-indigo-roscpp
Requires:       ros-indigo-scitos-2d-navigation
Requires:       ros-indigo-sensor-msgs
Requires:       ros-indigo-std-msgs
Requires:       ros-indigo-strands-description
Requires:       ros-indigo-tf
BuildRequires:  pcl-devel
BuildRequires:  ros-indigo-catkin
BuildRequires:  ros-indigo-pcl-ros
BuildRequires:  ros-indigo-roscpp
BuildRequires:  ros-indigo-sensor-msgs
BuildRequires:  ros-indigo-std-msgs
BuildRequires:  ros-indigo-tf

%description
This package contains components for using the ROS move base together with the
Scitos G5 robot. There is options for running obstacle avoidance both with only
laser and with an additional depth-sensing camera mounted in front. The
additional nodes in the package are for processing the incoming clouds from the
camera for obstacle avoidance.

%prep
%setup -q

%build
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree that was dropped by catkin, and source it.  It will
# set things like CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/indigo/setup.sh" ]; then . "/opt/ros/indigo/setup.sh"; fi
mkdir -p build && cd build
%cmake .. \
        -UINCLUDE_INSTALL_DIR \
        -ULIB_INSTALL_DIR \
        -USYSCONF_INSTALL_DIR \
        -USHARE_INSTALL_PREFIX \
        -ULIB_SUFFIX \
        -DCMAKE_INSTALL_PREFIX="/opt/ros/indigo" \
        -DCMAKE_PREFIX_PATH="/opt/ros/indigo" \
        -DSETUPTOOLS_DEB_LAYOUT=OFF \
        -DCATKIN_BUILD_BINARY_PACKAGE="1" \

make %{?_smp_mflags}

%install
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree that was dropped by catkin, and source it.  It will
# set things like CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/indigo/setup.sh" ]; then . "/opt/ros/indigo/setup.sh"; fi
cd build
make %{?_smp_mflags} install DESTDIR=%{buildroot}

%files
/opt/ros/indigo

%changelog
* Mon Oct 27 2014 nbore <nilsbore@gmail.com> - 0.0.6-0
- Autogenerated by Bloom

* Thu Oct 23 2014 nbore <nilsbore@gmail.com> - 0.0.5-0
- Autogenerated by Bloom

