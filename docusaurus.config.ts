import {themes as prismThemes} from 'prism-react-renderer';
import type {Config} from '@docusaurus/types';
import type * as Preset from '@docusaurus/preset-classic';
require('dotenv').config()

const config: Config = {
  title: 'LDYOM',
  tagline: 'Lua Design Your Own Mission',
  favicon: 'img/favicon.ico',

  // Set the production url of your site here
  url: 'https://getechg.github.io',
  // Set the /<baseUrl>/ pathname under which your site is served
  // For GitHub pages deployment, it is often '/<projectName>/'
  baseUrl: '/LDYOM/',

  // GitHub pages deployment config.
  // If you aren't using GitHub pages, you don't need these.
  organizationName: 'getechg', // Usually your GitHub org/user name.
  projectName: 'LDYOM', // Usually your repo name.
  deploymentBranch: 'gh-pages',

  onBrokenLinks: 'ignore',
  onBrokenMarkdownLinks: 'ignore',

  // Even if you don't use internationalization, you can use this field to set
  // useful metadata like html lang. For example, if your site is Chinese, you
  // may want to replace "en" with "zh-Hans".
  i18n: {
    defaultLocale: 'en',
    locales: ['en'],
  },

  presets: [
    [
      'classic',
      {
        docs: {
          sidebarPath: './sidebars.ts',
          // Please change this to your repo.
          // Remove this to remove the "edit this page" links.
          editUrl:
            'https://github.com/GeTechG/LDYOM/tree/site/',
        },
        blog: {
          showReadingTime: true,
          // Please change this to your repo.
          // Remove this to remove the "edit this page" links.
          editUrl:
            'https://github.com/GeTechG/LDYOM/tree/site/',
        },
        theme: {
          customCss: './src/css/custom.css',
        },
      } satisfies Preset.Options,
    ],
  ],

  themeConfig: {
    // Replace with your project's social card
    image: 'img/social-card.jpg',
    navbar: {
      logo: {
        alt: 'LDYOM Logo',
        src: 'img/logo.svg',
      },
      items: [
        {
          type: 'docSidebar',
          sidebarId: 'docs',
          position: 'left',
          label: 'Docs',
        },
        {to: '/blog', label: 'Blog', position: 'left'},
        {
          to: "/download",
          label: "Download",
          position: "left",
          className: "button download_button button--lg"
        },
        {
          href: 'https://github.com/GeTechG/LDYOM',
          label: 'GitHub',
          position: 'right',
        },
      ],
    },
    footer: {
      style: 'dark',
      links: [
        // {
        //   title: 'Docs',
        //   items: [
        //     {
        //       label: 'Tutorial',
        //       to: '/docs/intro',
        //     },
        //   ],
        // },
        {
          title: 'Community',
          items: [
            {
              label: 'VK',
              href: 'https://vk.com/ldyom',
            },
            {
              label: 'Discord',
              href: 'https://discord.gg/4KGf4Px',
            },
            {
                label: 'GTAForums',
                href: 'https://gtaforums.com/topic/973903-ldyom',
            },
            {
                label: 'GitHub',
                href: 'https://github.com/GeTechG/LDYOM',
            },
            {
                label: 'Telegram',
                href: 'https://t.me/ldyom',
            },
          ],
        },
        {
          title: 'More',
          items: [
            {
              label: 'Blog',
              to: '/blog',
            },
            {
              label: 'GitHub',
              href: 'https://github.com/GeTechG/LDYOM',
            },
          ],
        },
      ],
      copyright: `Copyright © ${new Date().getFullYear()} LDYOM.`,
    },
    prism: {
      theme: prismThemes.github,
      darkTheme: prismThemes.dracula,
    },
  } satisfies Preset.ThemeConfig,
};

export default config;
