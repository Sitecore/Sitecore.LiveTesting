#pragma once

#include "HostedWebCore.h"

namespace Sitecore
{
  namespace LiveTesting
  {
    namespace IIS
    {
      namespace Applications
      {
        public ref class IISTestApplicationManager : public Sitecore::LiveTesting::Applications::TestApplicationManager
        {
          private:
            literal System::String^ DEFAULT_HOSTED_WEB_CORE_INSTANCE_NAME = "Sitecore.LiveTesting";
            literal System::String^ APPLICATION_NAME_TEMPLATE = "/LM/W3SVC/{0}/ROOT{1}";
            literal System::String^ COLLECTION_ADD = "add";
            literal System::String^ COLLECTION_REMOVE = "remove";
            literal System::String^ COLLECTION_CLEAR = "clear";
            literal System::String^ SITE_ELEMENT_NAME = "site";
            literal System::String^ IIS_BIN_ENVIRONMENT_VARIABLE_TOKEN = "%IIS_BIN%";
            literal System::String^ SITE_ROOT_XPATH = "/configuration/system.applicationHost/sites";
            literal System::String^ APP_POOL_ROOT_XPATH = "/configuration/system.applicationHost/applicationPools";
            literal System::String^ DEFAULT_APP_POOL_XML = "<add name='Sitecore.LiveTesting' managedRuntimeVersion='v4.0' managedPipelineMode='Integrated' />";
            literal System::String^ DEFAULT_SITE_XML = "<site name='{1}' id='{1}' serverAutoStart='true'>{0}<bindings>{0}<binding protocol='http' bindingInformation='*:{2}:localhost' />{0}</bindings>{0}<application applicationPool='Sitecore.LiveTesting' path='/'>{0}<virtualDirectory path='/' physicalPath='' />{0}</application>{0}</site>";
            literal System::String^ DEFAULT_HOST_CONFIG_FILE_NAME = "Sitecore.LiveTesting.IIS.ApplicationHost.config";
            literal System::String^ SITE_SEARCH_TEMPLATE = "site[@name='{0}']";
            literal System::String^ SITE_NAME_ATTRIBUTE = "name";
            literal System::String^ SITE_APPLICATION_XPATH = "application";
            literal System::String^ DEFAULT_SITE_APPLICATION_XML = "<application applicationPool='Sitecore.LiveTesting' path='{1}'>{0}<virtualDirectory path='/' physicalPath='{2}' />{0}</application>";
            literal System::String^ ROOT_VIRTUAL_PATH = "/";
            literal System::String^ SITE_BINDING_XPATH = "bindings/binding[@protocol='http']/@bindingInformation";
            literal System::String^ SITE_BINDING_TEMPLATE = "*:{0}:localhost";
            literal System::String^ SINGLE_APP_POOL_XPATH = "/configuration/system.applicationHost/applicationPools/add[last()]/@name";

            static System::String^ ApplicationSiteName;
            static int ApplicationPort;

            initonly IIS::HostedWebCore^ m_hostedWebCore;

            static int GetFreePort();
            static System::AppDomain^ GetDefaultAppDomain();

            static void SetApplicationSiteName(System::String^ siteName);
            static void SetApplicationPort(int port);

            ref class ApplicationManagerProvider : public System::MarshalByRefObject
            {
              internal:
                System::Web::Hosting::ApplicationManager^ GetDefaultApplicationManager();
            };
          protected:
            property IIS::HostedWebCore^ HostedWebCore
            {
              IIS::HostedWebCore^ get();
            }

            static System::String^ GetDefaultHostConfigFileName();
            static System::String^ GetDefaultRootConfigFileName();
            static IIS::HostedWebCore^ GetHostedWebCoreForParametersOrDefaultIfAlreadyHosted(_In_ System::String^ hostConfig, _In_ System::String^ rootConfig, _In_ int connectionPoolSize);
            static System::Web::Hosting::ApplicationManager^ GetApplicationManagerFromDefaultAppDomain();

            virtual System::Xml::Linq::XDocument^ LoadHostConfiguration();
            virtual System::Xml::Linq::XElement^ GetSiteConfigurationForApplication(_In_ System::Xml::Linq::XDocument^ hostConfiguration, _In_ Sitecore::LiveTesting::Applications::TestApplicationHost^ applicationHost);
            virtual Sitecore::LiveTesting::Applications::TestApplicationHost^ AdjustApplicationHostToSiteConfiguration(_In_ System::Xml::Linq::XElement^ siteConfiguration);
            virtual void SetupApplicationEnvironment(_In_ Sitecore::LiveTesting::Applications::TestApplication^ application, _In_ System::Xml::Linq::XElement^ siteConfiguration);
            virtual void SaveHostConfiguration(_In_ System::Xml::Linq::XDocument^ hostConfiguration);
          public:
            IISTestApplicationManager(_In_ IIS::HostedWebCore^ hostedWebCore, _In_ System::Web::Hosting::ApplicationManager^ applicationManager, _In_ System::Type^ testApplicationType);
            IISTestApplicationManager(_In_ System::String^ hostConfig, _In_ System::String^ rootConfig, _In_ System::Type^ testApplicationType, _In_ int connectionPoolSize);
            IISTestApplicationManager(_In_ System::String^ hostConfig, _In_ System::String^ rootConfig);
            IISTestApplicationManager(_In_ System::String^ hostConfig, _In_ System::Type^ testApplicationType);
            IISTestApplicationManager(_In_ System::String^ hostConfig);
            IISTestApplicationManager(_In_ System::Type^ testApplicationType);
            IISTestApplicationManager();

            static System::String^ GetApplicationSiteName(_In_ Sitecore::LiveTesting::Applications::TestApplication^ application);
            static System::String^ GetApplicationVirtualPath(_In_ Sitecore::LiveTesting::Applications::TestApplication^ application);
            static System::String^ GetApplicationPhysicalPath(_In_ Sitecore::LiveTesting::Applications::TestApplication^ application);
            static int GetApplicationPort(_In_ Sitecore::LiveTesting::Applications::TestApplication^ application);

            Sitecore::LiveTesting::Applications::TestApplication^ StartApplication(_In_ Sitecore::LiveTesting::Applications::TestApplicationHost^ applicationHost) override;
        };
      }
    }
  }
}